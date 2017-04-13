#!/usr/bin/env python3
import socket
import threading

conn = None

def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        yield l[i:i + n]

def put(socket, key, val):
    message = "PUT:{}={}\n".format(key, val)
    socket.send(message.encode())

def get(socket, key):
    message = "GET:{}\n".format(key)
    socket.send(message.encode())
    reply = socket.recv(2048)
    print(reply.decode()[:-1])

def thread_work(words):
    for word in words:
        put(conn, word[:-1], "definition")

if __name__ == "__main__":
    conn = socket.create_connection(("localhost", 4567))
    with open("/usr/share/dict/words", "r") as f:
        words =  f.readlines()
    n_threads = 50
    words_per = int(len(words) / n_threads)
    for chunk in chunks(words, words_per):
        t = threading.Thread(target=thread_work, args=(chunk,))
        t.start()
