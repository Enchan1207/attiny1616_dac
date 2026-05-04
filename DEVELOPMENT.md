# Development information

## Read device signature

```sh
avrdude -p attiny1616 -c serialupdi -P /path/to/port -U signature:r:- | xxd
```

then:

```
Reading signature memory ...
Writing 3 bytes to output file <stdout>

Avrdude done.  Thank you.
00000000: 1e94 21  
```

## Configure, build

```sh
cmake --preset target
```

```sh
cmake --build --preset target
```

## Flash

```sh
avrdude -p attiny1616 -c serialupdi -P /path/to/port -U flash:w:./build/target/src/attiny1616_dac
```
