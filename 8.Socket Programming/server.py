import socket
import threading

HOST = '127.0.0.1' #local host
PORT = 8080 

clients = []

def broadcastmsg(message,sender_socket):
    for client in clients:
        if client == sender_socket:
            try:
                client.send(message)
            except:
                clients.remove(client)

def client_handler(client_socket):
    while True:
        try:
            message = client_socket.recv(1024)
            if not message :
                break
            print(f"Received message from client :{message.decode()}")
            broadcastmsg(message,client_socket)
        except:
            break

    clients.remove(client_socket)   
    client_socket.close()


def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(5)
    print(f'Server listening on {HOST}:{PORT}')

    while True:
        client_socket = server.accept()
        clients.append(client_socket)
        client_thread = threading.Thread(target=client_handler, args=(client_socket,))
        client_thread.start()

if __name__ == '__main__':
    main()

