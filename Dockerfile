FROM gcc:latest

WORKDIR /app

COPY src/ ./src/

RUN gcc src/*.c -o so-escalonador-grupo-3

CMD ["./so-escalonador-grupo-3"]