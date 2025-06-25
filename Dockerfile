# syntax=docker.io/docker/dockerfile:1.7-labs
# Copyright (c) 2019-2025 Toradex AG
# SPDX-License-Identifier: MIT

FROM torizon/wayland-base-dev-am62:4 as build
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libqt6opengl6-dev \
        qt6-base-dev \
        libqt6openglwidgets6 \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --exclude=build . .

RUN mkdir -p build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

FROM torizon/qt6-wayland-am62:4 as deploy

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libqt6opengl6 \
        libqt6openglwidgets6 \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /app/build/SpinningCube /app/
COPY --from=build /app/*.jpg /app/
COPY --from=build /app/*.png /app/

CMD ["./SpinningCube"]