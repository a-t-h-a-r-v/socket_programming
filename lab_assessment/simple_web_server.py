import socket
import os

def start_server(host='127.0.0.1', port=8080):
    # Create a TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Allow reuse of the address to avoid "address already in use" error
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    try:
        server_socket.bind((host, port))
        server_socket.listen(5)
        print(f"Server is listening on {host}:{port}")

        while True:
            # Wait for a connection
            connection_socket, addr = server_socket.accept()
            print(f"Connection established with {addr}")

            try:
                # Receive the request
                request = connection_socket.recv(1024).decode()
                print(f"Request received:\n{request}")

                if not request:
                    print("Empty request received.")
                    continue

                # Parse the request to get the file name
                request_line = request.splitlines()[0]
                request_parts = request_line.split()

                if len(request_parts) < 2:
                    print("Malformed request.")
                    continue

                file_name = request_parts[1]

                # If the file name is '/', serve index.html
                if file_name == '/':
                    file_name = '/index.html'

                # Remove the leading '/' from the file name
                file_path = file_name.lstrip('/')

                # Check if the file exists and read its content
                if os.path.exists(file_path) and os.path.isfile(file_path):
                    with open(file_path, 'rb') as file:
                        content = file.read()

                    # Create a valid HTTP response
                    response_headers = "HTTP/1.1 200 OK\r\n"
                    response_headers += f"Content-Length: {len(content)}\r\n"
                    response_headers += "Content-Type: text/html\r\n"
                    response_headers += "\r\n"
                    connection_socket.sendall(response_headers.encode() + content)
                else:
                    # Send a 404 Not Found response
                    error_message = "404 Not Found"
                    response_headers = "HTTP/1.1 404 Not Found\r\n"
                    response_headers += f"Content-Length: {len(error_message)}\r\n"
                    response_headers += "Content-Type: text/plain\r\n"
                    response_headers += "\r\n"
                    connection_socket.sendall(response_headers.encode() + error_message.encode())
            except Exception as e:
                print(f"Error handling request: {e}")
            finally:
                # Close the connection socket
                connection_socket.close()

    except Exception as e:
        print(f"Server error: {e}")
    finally:
        # Close the server socket on exit
        server_socket.close()

if __name__ == "__main__":
    start_server()
