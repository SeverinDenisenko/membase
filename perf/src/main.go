package main

import (
	"bufio"
	"fmt"
	"math"
	"math/rand"
	"net"
	"os"
	"time"
)

type Membase struct {
	conn   net.Conn
	reader bufio.Reader
}

type Hist struct {
	length        int
	buckets_names [16]string
	buckets       [16]float32
	counts        [16]int
	min           float32
	max           float32
	samples       int
	mean          float32
}

func Start() *Membase {
	r := new(Membase)
	address := "127.0.0.1:2222"

	var err error
	r.conn, err = net.Dial("tcp", address)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error connecting to Membase.\n")
		os.Exit(1)
	}

	r.reader = *bufio.NewReader(r.conn)

	return r
}

func (r *Membase) End() {
	defer r.conn.Close()
}

func (r *Membase) Send(request string) string {
	fmt.Fprintf(r.conn, request)
	response, err := r.reader.ReadString('\n')
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error reading response.\n")
		os.Exit(1)
	}
	return response
}

func RandString() string {
	var letterRunes = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
	b := make([]rune, 1024)
	for i := range b {
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	return string(b)
}

func (r *Membase) RunBatch(count int) int {
	start := time.Now()

	for i := 0; i < int(count); i++ {
		request := "PUT " + RandString() + " " + RandString()
		response := r.Send(request)
		if response == "ERROR\n" {
			fmt.Fprintf(os.Stderr, "Membase return error.\n")
			os.Exit(1)
		}
	}

	end := time.Now()
	return int(end.UnixMicro() - start.UnixMicro())
}

func CreateHist() *Hist {
	r := new(Hist)

	r.length = 16
	r.buckets_names = [16]string{
		"... <= 0.1 ms", "0.1 <= 0.2 ms", "0.2 <= 0.3 ms", "0.3 <= 0.4 ms",
		"0.4 <= 0.5 ms", "0.5 <= 0.6 ms", "0.6 <= 0.7 ms", "0.7 <= 0.8 ms",
		"0.8 <= 0.9 ms", "0.9 <= 1.0 ms", "1.0 <= 1.1 ms", "1.1 <= 1.2 ms",
		"1.2 <= 1.3 ms", "1.3 <= 1.4 ms", "1.4 <= 1.5 ms", "1.5 <= ... ms"}
	r.buckets = [16]float32{
		0.1, 0.2, 0.3, 0.4, 0.5,
		0.6, 0.7, 0.8, 0.9, 1.0,
		1.1, 1.2, 1.3, 1.4, 1.5, float32(math.Inf(1))}
	r.counts = [16]int{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

	return r
}

func (r *Hist) Insert(lat float32) {
	for i := 0; i < r.length; i++ {
		if lat <= r.buckets[i] {
			r.counts[i]++
			return
		}
	}
}

func (r *Hist) Print() {
	for i := 0; i < r.length; i++ {
		fmt.Println(r.buckets_names[i], r.counts[i])
	}

	fmt.Println("Samples: ", r.samples)
	fmt.Println("Min latency: ", r.min)
	fmt.Println("Max latency: ", r.max)
	fmt.Println("Mean latency: ", r.mean)
}

func (r *Membase) BuildHist(batches int, batch int) *Hist {
	res := CreateHist()

	var min float32 = float32(math.Inf(1))
	var max float32 = 0
	var mean float32 = 0

	for i := 0; i < batches; i++ {
		lat := float32(r.RunBatch(batch)) / float32(batch) / 1000
		min = float32(math.Min(float64(min), float64(lat)))
		max = float32(math.Max(float64(max), float64(lat)))
		mean += lat
		res.Insert(lat)
	}

	res.min = min
	res.max = max
	res.samples = batch * batches
	res.mean = mean / float32(res.samples)

	return res
}

func main() {
	membase := Start()
	hist := membase.BuildHist(5000, 1)
	membase.End()
	hist.Print()

	return
}
