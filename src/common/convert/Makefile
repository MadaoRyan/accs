LD=cc -fPIC -shared

up: libtl_cvt.so

clean:
	touch *.c

libtl_cvt.so:cJSON.c generate_json.c parse_json.c
	$(LD) -o $@ cJSON.c generate_json.c parse_json.c -I/home/tonglink/accs/include

	mv libtl_cvt.so ../../../lib
