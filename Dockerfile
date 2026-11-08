FROM gcc:latest

WORKDIR /app

COPY /src/* .

RUN gcc processo.c main.c -o so-escalonador-grupo-3

CMD ["./so-escalonador-grupo-3"]