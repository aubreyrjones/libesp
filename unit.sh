rm -f *.sqlite
rm -f *.esp
./tests/test1
./esp_uplift test1.esp
./sqlite session_stream.sqlite "select * from session_events;"
