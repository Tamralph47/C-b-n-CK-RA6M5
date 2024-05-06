const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.post('/api/data', (req, res) => {
    const inputData = req.body.inputData;
    // Xử lý dữ liệu đầu vào ở đây và phản hồi với dữ liệu mới
    res.json({ receivedData: inputData });
});

app.listen(port, () => {
    console.log(`API Backend đang lắng nghe tại http://localhost:${port}`);
});
