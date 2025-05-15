const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
  <meta charset="utf-8">
  <title>🌿 VƯỜN THÔNG MINH 🌿</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      background: linear-gradient(135deg, #e0f7fa 0%, #f1f8e9 100%);
      font-family: 'Segoe UI', Arial, sans-serif;
      margin: 0;
      padding: 0;
      color: #333;
    }
    .main-container {
      max-width: 480px;
      margin: 32px auto;
      background: #fff;
      border-radius: 18px;
      box-shadow: 0 4px 24px rgba(0,0,0,0.08);
      padding: 32px 24px 24px 24px;
    }
    h1 {
      text-align: center;
      color: #388e3c;
      margin-bottom: 12px;
      font-size: 2.1rem;
      letter-spacing: 1px;
    }
    h2 {
      color: #1976d2;
      margin-top: 24px;
      margin-bottom: 8px;
      font-size: 1.2rem;
      border-left: 4px solid #1976d2;
      padding-left: 8px;
    }
    label {
      font-weight: 500;
      margin-bottom: 4px;
      display: block;
      color: #388e3c;
    }
    input[type=text], input[type=number], select {
      width: 100%;
      padding: 12px;
      margin: 6px 0 16px 0;
      border: 1px solid #bdbdbd;
      border-radius: 8px;
      font-size: 1.1rem;
      background: #f9fbe7;
      transition: border 0.2s;
    }
    input[type=text]:focus, input[type=number]:focus, select:focus {
      border: 1.5px solid #1976d2;
      outline: none;
      background: #e3f2fd;
    }
    .container-2 {
      display: flex;
      gap: 12px;
      margin-bottom: 12px;
    }
    .container-2 input {
      width: 50%;
      margin: 0;
    }
    .submit {
      width: 100%;
      background: linear-gradient(90deg, #43a047 60%, #1976d2 100%);
      color: #fff;
      padding: 16px 0;
      border: none;
      border-radius: 8px;
      font-size: 1.2rem;
      font-weight: bold;
      cursor: pointer;
      margin-top: 18px;
      box-shadow: 0 2px 8px rgba(67,160,71,0.08);
      transition: background 0.2s;
    }
    .submit:hover {
      background: linear-gradient(90deg, #1976d2 60%, #43a047 100%);
    }
    .note {
      font-size: 0.95rem;
      color: #757575;
      margin-bottom: 10px;
      margin-top: -8px;
    }
    @media (max-width: 600px) {
      .main-container {
        padding: 16px 4vw 18px 4vw;
      }
      h1 { font-size: 1.3rem; }
      h2 { font-size: 1rem; }
    }
  </style>
</head>
<body>
  <div class="main-container">
    <h1>🌿 HỆ THỐNG GIÁM SÁT VƯỜN THÔNG MINH 🌿</h1>
    <form id="configForm" autocomplete="off">
      <h2>Cấu hình WiFi</h2>
      <label for="ssid">Tên WiFi</label>
      <input type="text" id="ssid" name="ssid" placeholder="Nhập tên WiFi...">
      <label for="pass">Mật khẩu</label>
      <input type="text" id="pass" name="pass" placeholder="Nhập mật khẩu...">
      <label for="token">Mã Token Blynk</label>
      <input type="text" id="token" name="token" placeholder="Nhập Token Blynk...">

      <h2>Cấu hình ngưỡng</h2>
      <label for="caytrong">Chọn loại cây trồng</label>
      <select name="caytrong" id="caytrong" onchange="checkUserSelected()">
        <option value="0">Tự cài đặt</option>
        <option value="1">Rau mầm</option>
        <option value="2">Bắp cải</option>
        <option value="3">Cà chua</option>
        <option value="4">Xà lách</option>
        <option value="5">Dưa chuột</option>
        <option value="6">Sen đá</option>
        <option value="7">Rau mùi</option>
      </select>

      <label>Ngưỡng nhiệt độ môi trường (°C)</label>
      <div class="note">🌞 Ngưỡng 1 &lt; Khoảng an toàn &lt; Ngưỡng 2</div>
      <div class="container-2">
        <input class="tempThre" type="number" id="tempThreshold1" name="tempThreshold1" min="10" max="100" step="1" placeholder="Ngưỡng 1">
        <input class="tempThre" type="number" id="tempThreshold2" name="tempThreshold2" min="10" max="100" step="1" placeholder="Ngưỡng 2">
      </div>

      <label>Ngưỡng độ ẩm không khí (%)</label>
      <div class="note">🌱 Ngưỡng 1 &lt; Khoảng an toàn &lt; Ngưỡng 2</div>
      <div class="container-2">
        <input class="humiThre" type="number" id="humiThreshold1" name="humiThreshold1" min="10" max="100" step="1" placeholder="Ngưỡng 1">
        <input class="humiThre" type="number" id="humiThreshold2" name="humiThreshold2" min="10" max="100" step="1" placeholder="Ngưỡng 2">
      </div>

      <label>Ngưỡng độ ẩm đất (%)</label>
      <div class="note">🍁 Ngưỡng 1 &lt; Khoảng an toàn &lt; Ngưỡng 2</div>
      <div class="container-2">
        <input class="soilThre" type="number" id="soilMoistureThreshold1" name="soilMoistureThreshold1" placeholder="Ngưỡng 1">
        <input class="soilThre" type="number" id="soilMoistureThreshold2" name="soilMoistureThreshold2" placeholder="Ngưỡng 2">
      </div>

      <button class="submit" id="btnSubmit" type="button">Lưu cấu hình</button>
    </form>
  </div>
  <script>
    var data = {
      ssid   : "",
      pass   : "",
      token : "",
      typePlant : "",
      tempThreshold1 : "",
      tempThreshold2 : "",
      humiThreshold1 : "",
      humiThreshold2 : "",
      soilMoistureThreshold1 : "",
      soilMoistureThreshold2 : "",
    };
    const ssid      = document.getElementById("ssid");
    const pass      = document.getElementById("pass");
    const token     = document.getElementById("token");
    const typePlant = document.getElementById("caytrong");

    const tempThreshold1 =  document.getElementsByName('tempThreshold1')[0];
    const tempThreshold2 =  document.getElementsByName('tempThreshold2')[0];
    const humiThreshold1 = document.getElementsByName('humiThreshold1')[0];
    const humiThreshold2 = document.getElementsByName('humiThreshold2')[0];
    const soilMoistureThreshold1 = document.getElementsByName('soilMoistureThreshold1')[0];
    const soilMoistureThreshold2 = document.getElementsByName('soilMoistureThreshold2')[0];

    // Khởi tạo các giá trị ngưỡng của thực vật
    function PLANT(tempThreshold1, tempThreshold2, humiThreshold1, humiThreshold2, soilMoistureThreshold1, soilMoistureThreshold2) {
      this.tempThreshold1 = tempThreshold1;
      this.tempThreshold2 = tempThreshold2;
      this.humiThreshold1 = humiThreshold1;
      this.humiThreshold2 = humiThreshold2;
      this.soilMoistureThreshold1 = soilMoistureThreshold1;
      this.soilMoistureThreshold2 = soilMoistureThreshold2;
    }
    raumam   = new PLANT(20,25,50,70,60,80);
    bapcai   = new PLANT(15,21,60,80,60,80);
    cachua   = new PLANT(15,25,60,80,60,80);
    xalach   = new PLANT(15,20,60,80,60,80);
    duachuot = new PLANT(20,25,60,80,60,80);
    senda    = new PLANT(15,25,30,50,30,50);
    raumui   = new PLANT(15,20,50,70,60,80);

    // lấy data ban đầu
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET","/data_before", true),
    xhttp.send();
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState == 4 && xhttp.status == 200) {
        const obj    = JSON.parse(this.responseText);
        ssid.value   = obj.ssid;
        pass.value   = obj.pass;
        token.value  = obj.token;
        typePlant.value = obj.typePlant;
        tempThreshold1.value = obj.tempThreshold1;
        tempThreshold2.value = obj.tempThreshold2;
        humiThreshold1.value = obj.humiThreshold1;
        humiThreshold2.value = obj.humiThreshold2;
        soilMoistureThreshold1.value = obj.soilMoistureThreshold1;
        soilMoistureThreshold2.value = obj.soilMoistureThreshold2;
        document.getElementById("caytrong").options.selectedIndex = Number(obj.typePlant);
        checkPlantAndPutValue(Number(obj.typePlant));
      }
    }

    function checkUserSelected() {
      var index = typePlant.options.selectedIndex;
      checkPlantAndPutValue(index);
    }

    function checkPlantAndPutValue(value) {
      if(value === 0) {
        document.getElementById("tempThreshold1").disabled = false;
        document.getElementById("tempThreshold2").disabled = false;
        document.getElementById("humiThreshold1").disabled = false;
        document.getElementById("humiThreshold2").disabled = false;
        document.getElementById("soilMoistureThreshold1").disabled = false;
        document.getElementById("soilMoistureThreshold2").disabled = false;
      } else {
        document.getElementById("tempThreshold1").disabled = true;
        document.getElementById("tempThreshold2").disabled = true;
        document.getElementById("humiThreshold1").disabled = true;
        document.getElementById("humiThreshold2").disabled = true;
        document.getElementById("soilMoistureThreshold1").disabled = true;
        document.getElementById("soilMoistureThreshold2").disabled = true;
        switch (value) {
          case 1:
            tempThreshold1.value = raumam.tempThreshold1;
            tempThreshold2.value = raumam.tempThreshold2;
            humiThreshold1.value = raumam.humiThreshold1;
            humiThreshold2.value = raumam.humiThreshold2;
            soilMoistureThreshold1.value = raumam.soilMoistureThreshold1;
            soilMoistureThreshold2.value = raumam.soilMoistureThreshold2;
            break;
          case 2:
            tempThreshold1.value = bapcai.tempThreshold1;
            tempThreshold2.value = bapcai.tempThreshold2;
            humiThreshold1.value = bapcai.humiThreshold1;
            humiThreshold2.value = bapcai.humiThreshold2;
            soilMoistureThreshold1.value = bapcai.soilMoistureThreshold1;
            soilMoistureThreshold2.value = bapcai.soilMoistureThreshold2;
            break;
          case 3:
            tempThreshold1.value = cachua.tempThreshold1;
            tempThreshold2.value = cachua.tempThreshold2;
            humiThreshold1.value = cachua.humiThreshold1;
            humiThreshold2.value = cachua.humiThreshold2;
            soilMoistureThreshold1.value = cachua.soilMoistureThreshold1;
            soilMoistureThreshold2.value = cachua.soilMoistureThreshold2;
            break;
          case 4:
            tempThreshold1.value = xalach.tempThreshold1;
            tempThreshold2.value = xalach.tempThreshold2;
            humiThreshold1.value = xalach.humiThreshold1;
            humiThreshold2.value = xalach.humiThreshold2;
            soilMoistureThreshold1.value = xalach.soilMoistureThreshold1;
            soilMoistureThreshold2.value = xalach.soilMoistureThreshold2;
            break;
          case 5:
            tempThreshold1.value = duachuot.tempThreshold1;
            tempThreshold2.value = duachuot.tempThreshold2;
            humiThreshold1.value = duachuot.humiThreshold1;
            humiThreshold2.value = duachuot.humiThreshold2;
            soilMoistureThreshold1.value = duachuot.soilMoistureThreshold1;
            soilMoistureThreshold2.value = duachuot.soilMoistureThreshold2;
            break;
          case 6:
            tempThreshold1.value = senda.tempThreshold1;
            tempThreshold2.value = senda.tempThreshold2;
            humiThreshold1.value = senda.humiThreshold1;
            humiThreshold2.value = senda.humiThreshold2;
            soilMoistureThreshold1.value = senda.soilMoistureThreshold1;
            soilMoistureThreshold2.value = senda.soilMoistureThreshold2;
            break;
          case 7:
            tempThreshold1.value = raumui.tempThreshold1;
            tempThreshold2.value = raumui.tempThreshold2;
            humiThreshold1.value = raumui.humiThreshold1;
            humiThreshold2.value = raumui.humiThreshold2;
            soilMoistureThreshold1.value = raumui.soilMoistureThreshold1;
            soilMoistureThreshold2.value = raumui.soilMoistureThreshold2;
            break;
        }
      }
    }

    var xhttp2 = new XMLHttpRequest();
    const btnSubmit = document.getElementById("btnSubmit");
    btnSubmit.addEventListener('click', () => {
      data = {
        ssid   : ssid.value,
        pass   : pass.value,
        token  :  token.value,
        typePlant :  Number(typePlant.value),
        tempThreshold1 : Number(tempThreshold1.value),
        tempThreshold2 : Number(tempThreshold2.value),
        humiThreshold1 : Number(humiThreshold1.value),
        humiThreshold2 : Number(humiThreshold2.value),
        soilMoistureThreshold1 : Number(soilMoistureThreshold1.value),
        soilMoistureThreshold2 : Number(soilMoistureThreshold2.value)
      }
      xhttp2.open("POST","/post_data", true),
      xhttp2.send(JSON.stringify(data));
      xhttp2.onreadystatechange = function() {
        if(xhttp2.readyState == 4 && xhttp2.status == 200) {
          alert("Cài đặt thành công");
        }
      }
    });
  </script>
</body>
</html>
)rawliteral";