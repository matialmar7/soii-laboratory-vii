docker run -dti --rm --privileged \
    --shm-size=1g --ulimit memlock=-1 --ulimit stack=67108864 \
    --network host \
    --env "TERM=xterm-256color" \
    --name "so2-dev"\
    --volume $2:/home/so:rw \
    -w "/home/so" \
    ubuntu bash