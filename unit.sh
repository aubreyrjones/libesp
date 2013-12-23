rm -f *.sqlite
rm -f *.esp
./tests/test2
./esp_uplift test2.esp
./sqlite test2.esp.sqlite "select * from events;"
./sqlite test2.esp.sqlite "select * from strings;"
