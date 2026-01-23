#include "config/IndexPage.hpp"
#include <string>

static const std::string index_page = R""""(
<!DOCTYPE html>
<html>
  <head>
    <title>CU Configuration</title>
</head>
<body>
  <h1>Configuration</h1>

  <form id="wifi-form">
    <h2>Wifi Configuration</h2>

    <div>
      <label for="wifi-ssid">SSID</label>
      <input id="wifi-ssid" name="wifi-ssid" type="text" value="">
    </div>
    <div>
      <label for="wifi-psk">PSK</label>
      <input id="wifi-psk" name="wifi-psk" type="text" value="">
    </div>

    <div>
      <button id="wifi-submit" type="submit">Save</button>
    </div>
  </form>

  <script>
    window.onload = (event) => {
      const wifiForm = document.querySelector("#wifi-form");
      if (wifiForm) {
        wifiForm.addEventListener("submit", function (e) {
          submitForm(e, this);
        });
      }

      loadConfigs();
    };


    async function loadConfigs() {
      const configs = await fetchConfigs();
      for (const config of configs) {
        if (config.name === "wifi") {
          document.getElementById("wifi-ssid").value = config.config.ssid;
          document.getElementById("wifi-psk").value = config.config.psk;
        }
      }
    }

    async function submitForm(e, form) {
      e.preventDefault();

      const btn = document.getElementById('wifi-submit');
      btn.disabled = true;
      setTimeout(() => btn.disabled = false, 2000);

      const data = buildJsonData(form);

      const response = await saveConfig('wifi', await data);
    }

    function buildJsonData(form) {
      const ssid = document.getElementById("wifi-ssid").value;
      const psk = document.getElementById("wifi-psk").value;

      return { ssid, psk };
    }

    async function fetchConfigs() {
        try {
            const response = await fetch('/api/config');
            return await response.json();
        } catch (err) {
            console.log('Fetch error: ' + err);
        }
    }

    async function saveConfig(name, config) {
      try {
        const response = await fetch('/api/config/save', {
          method: "POST",
          body: JSON.stringify(config)
        });

        alert('Saved');
      } catch (err) {
        alert('Fetch error');
        console.log('Fetch error: ' + err);
      }
    }
  </script>
  <style>
    body {
        color: white;
        background-color: darkslategrey;

        width: 100%;
        display: flex;
        flex-direction: column;
        justify-content: center;
    }
    form {
        display: inline-flex;
        flex-direction: column;
    }
  </style>
</body>
</html>
)"""";

const std::string getIndexHtml()
{
    return index_page;
}
