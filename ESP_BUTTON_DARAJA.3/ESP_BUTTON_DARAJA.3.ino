#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Psalmchmart";
const char* password = "Psalmucho@2040";

const char* PARAM_INPUT_1 = "amount";
const char* PARAM_INPUT_2 = "phone";

const int buttonPin = 2;
volatile bool buttonPressed = false;

int buttonState;            
int lastButtonState = LOW;  

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Lipa na M-Pesa</title>
  <link href="" rel="stylesheet" />
  <!-- CSS only -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" ">
  <style>
    @import url("https://fonts.googleapis.com/css2?family=Rubik:wght@500&display=swap");
    body {
      background-color: #eaedf4;
      font-family: "Rubik", sans-serif;
    }
    .card {
      width: 310px;
      border: none;
      border-radius: 15px;
    }
    .justify-content-around div {
      border: none;
      border-radius: 20px;
      background: #f3f4f6;
      padding: 5px 20px 5px;
      color: #8d9297;
    }
    .justify-content-around span {
      font-size: 12px;
    }
    .justify-content-around div:hover {
      background: #545ebd;
      color: #fff;
      cursor: pointer;
    }
    .justify-content-around div:nth-child(1) {
      background: #545ebd;
      color: #fff;
    }
    span.mt-0 {
      color: #8d9297;
      font-size: 12px;
    }
    h6 {
      font-size: 15px;
    }
    .mpesa {
      background-color: green !important;
    }
    img {
      border-radius: 15px;
    }
  </style>
</head>
<body class="snippet-body">
  <div class="container d-flex justify-content-center">
    <div class="card mt-5 px-3 py-4">
      <div class="d-flex flex-row justify-content-around">
        <div class="mpesa"><span>Mpesa </span></div>
        <div><span>Paypal</span></div>
        <div><span>Card</span></div>
      </div>
      <div class="media mt-4 pl-2">
        <div class="media-body">
          <h6 class="mt-1">Enter Amount & Number</h6>
        </div>
      </div>
      <div class="media mt-3 pl-2">
        <form class="row g-3" id="myForm" action="/api/stkPush" method="POST">
          <div class="col-12">
            <label for="amount" class="form-label">Amount</label>
            <input type="text" id="amount" class="form-control" name="amount" placeholder="Enter Amount">
          </div>
          <div class="col-12">
            <label for="phone" class="form-label">Phone Number</label>
            <input type="text" id="phone" class="form-control" name="phone" placeholder="Enter Phone Number">
          </div>
          <div class="col-12">
            <button type="submit" class="btn btn-success" name="submit" value="submit">Submit</button>
          </div>
        </form>
      </div>
    </div>
  </div>
  <script>
    document.getElementById('myForm').addEventListener('submit', function(event) {
      event.preventDefault();
      const amount = document.getElementById("amount").value;
      const phone = document.getElementById("phone").value;
      const formData = { amount, phone };
      fetch("/api/stkPush", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(formData)
      })
      .then(response => {
        if (response.ok) {
          console.log("Data sent successfully");
        } else {
          console.error("Failed to send data");
        }
      })
      .catch(error => {
        console.error("An error occurred while sending data", error);
      });
    });
  </script>
</body>
</html>
)rawliteral";

void handleRootRequest(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html);
}

void handleFormSubmit(AsyncWebServerRequest *request) {
  String inputAmount;
  String inputNumber;

  if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
    inputAmount = request->getParam(PARAM_INPUT_1)->value();
    inputNumber = request->getParam(PARAM_INPUT_2)->value();
  }
  else {
    inputAmount = "No message sent";
    inputNumber = "No message sent";
  }

  String responseMessage = "HTTP POST request sent to your ESP with values: Amount - " + inputAmount + ", Phone Number - " + inputNumber;
  request->send(200, "text/plain", responseMessage);
}

void buttonInterrupt() {
  buttonPressed = true;
}
void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, CHANGE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRootRequest);
  server.on("/api/stkPush", HTTP_POST, handleFormSubmit);

  server.begin();
}

void loop() {
  
}
