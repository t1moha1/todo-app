FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y build-essential cmake libpqxx-dev

WORKDIR /app
COPY . /app

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j

EXPOSE 8080
CMD ["./build/todo-server"]