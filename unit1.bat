cd tests
del *.esp
test1.exe
cd ..
del *.sqlite
esp_uplift.exe tests/test1.esp
sqlite.exe session_stream.sqlite "select * from events where value > 10;"


