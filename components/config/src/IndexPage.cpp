#include "config/IndexPage.hpp"
#include <string>

static const std::string index_page = R""""(
<!DOCTYPE html>
<html>
  <head>
    <title>CU Configuration</title>
</head>
<body>
  <div class="container">
    <h1>Configuration</h1>

    <form id="wifi-form">
      <h2>Wifi Configuration</h2>

      <div class="form-group">
        <label for="wifi-ssid">SSID</label>
        <input id="wifi-ssid" name="wifi-ssid" class="form-control" type="text" value="" required>
      </div>
      <div class="form-group">
        <label for="wifi-psk">PSK</label>
        <input id="wifi-psk" name="wifi-psk" class="form-control" type="text" value="" minlength="6" required>
      </div>

      <div>
        <button id="wifi-submit" type="submit" class="btn btn-primary">Save</button>
      </div>

      <div>
        <span id="wifi-succeed" class="hidden text-succeed">Saved</span>
        <span id="wifi-error" class="hidden text-error">Error</span>
      </div>
    </form>
  </div>

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

      document.getElementById('wifi-succeed').classList.add('hidden');
      document.getElementById('wifi-error').classList.add('hidden');

      const btn = document.getElementById('wifi-submit');
      btn.disabled = true;
      setTimeout(() => btn.disabled = false, 2000);

      const data = formData();

      const result = await saveConfig('wifi', await data);
      if (result) {
          document.getElementById('wifi-succeed').classList.remove('hidden');
      } else {
          document.getElementById('wifi-error').classList.remove('hidden');
      }
    }

    function formData() {
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
          body: JSON.stringify(config),
          signal: AbortSignal.timeout(1000)
        });

        return true;
      } catch (err) {
        console.log('Fetch error: ' + err);
        return false;
      }
    }
  </script>
  <style>
    body {
        color: white;
        background-color: darkslategrey;

        width: 100%;

        font-size: 3rem;
        font-family: arial;

        text-align: center;

        display: flex;
        justify-content: center;
    }

    .container {
        display: flex;
        flex-direction: column;
    }

    form {
        display: flex;
        flex-direction: column;
        gap: 2rem;

        width: 40rem;
    }

    .form-group {
        gap: 0.5rem;
    }

    .form-control {
        display: block;
        font-size: 3rem;
        border-radius: 0.5rem;
        padding: 0.5rem
    }

    .btn {
        font-size: 3rem;
        display: block;
        border: none;
        border-radius: 0.5rem;
        padding: 0.5rem
    }

    .btn-primary {
        color: white;
        background-color: mediumslateblue;
    }

    h1 {
        font-size: 5rem;
    }

    h2 {
        font-size: 4rem;
    }

    div {
        display: flex;
        flex-direction: column;
        padding: 0.25rem 0.5rem
    }

    .hidden {
        display: none;
    }

    .text-succeed {
        color: lawngreen;
    }

    .text-error {
        color: red;
    }
  </style>
</body>
</html>
)"""";

const std::string getIndexHtml()
{
    return index_page;
}
