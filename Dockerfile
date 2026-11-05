FROM gcc:latest

WORKDIR /app

COPY . .

RUN gcc main.c -o escalonador

CMD ["./escalonador"]