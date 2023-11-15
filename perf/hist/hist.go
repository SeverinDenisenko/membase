package hist

import (
	"fmt"
	"math"
)

type Hist struct {
	length        int
	buckets_names []string
	buckets       []float32
	counts        []int
	min           float32
	max           float32
	samples       int
	mean          float32
	sum           float32
}

func CreateHist(begin float32, end float32, step float32) *Hist {
	r := new(Hist)

	r.length = int((end - begin) / step)

	r.buckets_names = append(r.buckets_names, fmt.Sprintf(".... ... %.2f ms: ", begin))
	r.buckets = append(r.buckets, float32(math.Inf(-1)))
	r.counts = append(r.counts, 0)
	for i := 0; i < r.length; i++ {
		left := begin + step*float32(i)
		rigth := begin + step*float32(i+1)
		r.buckets_names = append(r.buckets_names, fmt.Sprintf("%.2f ... %.2f ms: ", left, rigth))
		r.buckets = append(r.buckets, rigth)
		r.counts = append(r.counts, 0)
	}
	r.buckets_names = append(r.buckets_names, fmt.Sprintf("%.2f ... .... ms: ", end))
	r.buckets = append(r.buckets, float32(math.Inf(1)))
	r.counts = append(r.counts, 0)

	r.max = float32(math.Inf(-1))
	r.min = float32(math.Inf(1))

	return r
}

func (r *Hist) Print() {
	for i := 0; i < r.length+2; i++ {
		fmt.Println(r.buckets_names[i], r.counts[i])
	}

	fmt.Printf("Samples: %d\n", r.samples)
	fmt.Printf("Min latency: %.3f\n", r.min)
	fmt.Printf("Max latency: %.3f\n", r.max)
	fmt.Printf("Mean latency: %.3f\n", r.mean)
}

func (r *Hist) Insert(lat float32) {
	r.min = float32(math.Min(float64(r.min), float64(lat)))
	r.max = float32(math.Max(float64(r.max), float64(lat)))
	r.samples++
	r.sum += lat
	r.mean = r.sum / float32(r.samples)

	for i := 0; i < r.length+2; i++ {
		if lat <= r.buckets[i] {
			r.counts[i]++
			return
		}
	}
}
