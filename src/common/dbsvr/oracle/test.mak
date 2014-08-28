cc -fPIC -shared -o libdbs_oracle.so -I/oracle/product/10g/precomp/public -I/oracle/product/10g/lib -I/home/tonglink/accs/include/incl_base -L/oracle/product/10g/lib -lclntsh -lsql10 -lm -lcurses dbs_oracle.c \
        dbs_0.c \
        dbs_1.c \
        dbs_2.c \
        dbs_3.c \
        dbs_4.c \
        dbs_5.c \
        dbs_6.c \
        dbs_7.c \
        dbs_8.c \
        dbs_9.c \
        dbs_10.c \
        dbs_11.c \
        dbs_12.c \
        dbs_13.c \
        dbs_14.c \
        dbs_15.c \
        dbs_16.c \
        dbs_17.c \
        dbs_18.c \
        dbs_19.c \
        dbs_20.c \
        dbs_oraoper.c \
        dbs_page.c
