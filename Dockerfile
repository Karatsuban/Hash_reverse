# syntax=docker/dockerfile:1

FROM gcc:latest
WORKDIR /home
COPY . .
RUN make
