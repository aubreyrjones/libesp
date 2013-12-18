rm session_stream.sqlite
rm session_stream.esp
./tests/test1
./esp_uplift session_stream.esp
./sqlite session_stream.sqlite "select * from session_events;"
