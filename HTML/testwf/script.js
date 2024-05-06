// JavaScript code to fetch data from ESP32 and update HTML elements
setInterval(function() {
    fetch('fe80::18ae:11ff:feef:5a12') // Địa chỉ để ESP32 cung cấp dữ liệu, ví dụ '/data'
    .then(response => response.json())
    .then(data => {
        document.getElementById('temperature').innerText = 'Nhiệt độ: ' + data.temperature + ' °C';
        document.getElementById('humidity').innerText = 'Độ ẩm: ' + data.humidity + ' %';
    })
    .catch(error => console.error('Error:', error));
}, 2000); // Lặp lại mỗi 2 giây