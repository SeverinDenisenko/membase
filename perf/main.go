package main

import (
	"fmt"
	"math/rand"
	hist "perf/hist"
	membase "perf/membase"
	"time"
)

func RandString() string {
	var letterRunes = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
	b := make([]rune, 1024)
	for i := range b {
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	return string(b)
}

func TestPUT(mb *membase.Membase) *hist.Hist {
	var begin float32 = 0.5
	var end float32 = 5.0
	var step float32 = 0.25
	var count int = 100000
	hist := hist.CreateHist(begin, end, step)

	for i := 0; i < count; i++ {
		request := "PUT " + RandString() + " " + RandString()
		start := time.Now().UnixMicro()
		mb.SendCheck(request)
		finish := time.Now().UnixMicro()
		hist.Insert(float32(finish-start) / 1000)
	}

	return hist
}

func TestGET(mb *membase.Membase) *hist.Hist {
	var begin float32 = 0.5
	var end float32 = 5.0
	var step float32 = 0.25
	var count int = 100000
	hist := hist.CreateHist(begin, end, step)

	for i := 0; i < count; i++ {
		key := RandString()
		value := RandString()
		request := "PUT " + key + " " + value
		mb.SendCheck(request)
		request = "GET " + key
		response := "VALUE " + value + "\n"
		start := time.Now().UnixMicro()
		mb.SendExpect(request, response)
		finish := time.Now().UnixMicro()
		hist.Insert(float32(finish-start) / 1000)
	}

	return hist
}

func main() {
	mb := membase.Start("127.0.0.1:2222")

	fmt.Println("Testing PUT...")
	hist := TestPUT(mb)
	hist.Print()

	fmt.Println("Testing GET...")
	hist = TestGET(mb)
	hist.Print()

	mb.End()
}
