import socket
import threading

SERVER_IP = '127.0.0.1'
PORT = 8080

# Function to receive messages from the server
def receive_message(client_socket):
    while True:
        try:
            message = client_socket.recv(1024)
            print(message.decode())
        except:
            print("Connection to the server is closed.")
            client_socket.close()
            break

# Main function to set up the client
def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((SERVER_IP, PORT))

    # Start a thread for receiving messages
    receive_thread = threading.Thread(target=receive_message, args=(client,))
    receive_thread.start()

    # Loop to send messages to the server
    while True:
        message = input("Enter message to broadcast: ")
        if message.lower() == 'quit':
            break
        client.send(message.encode())
    client.close()

if __name__ == '__main__':
    main()
