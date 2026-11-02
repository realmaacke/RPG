default: (build "all")

# Setup default build directory using Ninja
setup service='all':
    #!/usr/bin/env bash
    set -eo pipefail

    SERVICE="{{ service }}"

    if [[ "$SERVICE" == "all" ]]; then
        (cd game && meson setup builddir --backend=ninja)
        (cd server && meson setup builddir --backend=ninja)
    elif [[ "$SERVICE" == "game" ]]; then
        (cd game && meson setup builddir --backend=ninja)
    elif [[ "$SERVICE" == "server" ]]; then
        (cd server && meson setup builddir --backend=ninja)
    else
        echo "Unknown service: $SERVICE"
        exit 1
    fi

# Compile the project
build service='all':
    #!/usr/bin/env bash
    set -eo pipefail

    SERVICE="{{ service }}"

    if [[ "$SERVICE" == "all" ]]; then
        meson compile -C game/builddir
        meson compile -C server/builddir
    elif [[ "$SERVICE" == "game" ]]; then
        meson compile -C game/builddir
    elif [[ "$SERVICE" == "server" ]]; then
        meson compile -C server/builddir
    else
        echo "Unknown service: $SERVICE"
        exit 1
    fi

# Run a service
run service='server': (build service)
    #!/usr/bin/env bash
    set -eo pipefail

    SERVICE="{{ service }}"

    if [[ "$SERVICE" == "game" ]]; then
        ./game/builddir/game
    elif [[ "$SERVICE" == "server" ]]; then
        ./server/builddir/server
    else
        echo "Can only run 'game' or 'server'"
        exit 1
    fi

# Clean build artifacts
clean service='all':
    #!/usr/bin/env bash
    SERVICE="{{ service }}"
    if [[ "$SERVICE" == "all" || "$SERVICE" == "game" ]]; then
        rm -rf game/builddir
    fi
    if [[ "$SERVICE" == "all" || "$SERVICE" == "server" ]]; then
        rm -rf server/builddir
    fi