# mkdisk sh script file:

  sudo dd if=/dev/zero of=$1 bs=1024 count=1440
  sudo mkfs $1 1440