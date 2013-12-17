rm esp_session.sqlite
./tests/test1
./sqlite esp_session.sqlite "select * from session_events;"
