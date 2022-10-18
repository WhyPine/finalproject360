# sh script do_it:
		    
    sudo mount $1 /mnt
    (cd /mnt; ls -l; sudo mkdir dir1; sudo touch file1)
    sudo umount $1