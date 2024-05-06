#include <Arduino.h>

#include <DHT.h>

#define DHTPIN 26     // Chân kết nối cảm biến DHT11 với ESP32
#define DHTTYPE DHT11   // Loại cảm biến (DHT11 hoặc DHT22)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000);  // Đợi 2 giây giữa mỗi lần đọc dữ liệu

  // Đọc độ ẩm từ cảm biến
  float humidity = dht.readHumidity();
  // Đọc nhiệt độ từ cảm biến
  float temperature = dht.readTemperature();

  // Kiểm tra nếu đọc dữ liệu thành công
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Lỗi khi đọc dữ liệu từ cảm biến DHT!");
    return;
  }

  // In ra nhiệt độ và độ ẩm
  Serial.print("Nhiệt độ: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Độ ẩm: ");
  Serial.print(humidity);
  Serial.println(" %");
}