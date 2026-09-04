FROM silkeh/clang:latest

USER root
RUN apt-get update && \
    apt-get install -y valgrind && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
