rm -f *.sqlite
rm -f *.esp
./tests/test2
./esp_uplift test2.esp
./sqlite session_stream.sqlite "select * from session_events;"
./sqlite session_stream.sqlite "select * from session_strings;"
