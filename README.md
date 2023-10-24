# Assignment 4

## Distributed File Sharing System

### Description
- 2 folders :-
- client - contains all the peer files
- tracker - contains all the tracker related files

### execution
run make file in each folder on a different terminal
- client
```
make
./a.out 127.0.0.1:8010 tracker_info.txt
```

- tracker 
```
make
./a.out tracker_info.txt 1
```



