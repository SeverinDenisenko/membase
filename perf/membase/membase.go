package membase

import (
	"bufio"
	"fmt"
	"net"
	"os"
)

type Membase struct {
	conn   net.Conn
	reader bufio.Reader
}

func Start(address string) *Membase {
	r := new(Membase)

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
	fmt.Fprint(r.conn, request)
	response, err := r.reader.ReadString('\n')
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error reading response.\n")
		os.Exit(1)
	}
	return response
}

func (r *Membase) SendCheck(request string) {
	response := r.Send(request)
	if response == "ERROR\n" {
		fmt.Fprintf(os.Stderr, "Membase return error.\n")
		os.Exit(1)
	}
}

func (r *Membase) SendExpect(request string, response string) {
	returned := r.Send(request)
	if returned != response {
		fmt.Fprintf(os.Stderr, "Membase return unexpected result.\n")
		fmt.Fprintf(os.Stderr, "%s != %s", response, returned)
		os.Exit(1)
	}
}
