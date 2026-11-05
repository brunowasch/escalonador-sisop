FROM gcc:latest

WORKDIR /app

COPY /src/* .

RUN gcc main.c -o escalonador

CMD ["./escalonador"]