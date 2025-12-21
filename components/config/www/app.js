window.addEventListener('DOMContentLoaded', function() {
    loadConfig();
});

function loadConfig() {
    document.getElementById('wifi_loading').style.display = 'block';

    fetch('/api/config/wifi')
        .then(function(response) {
            if (!response.ok) {
                throw new Error('Fetch error: ' + response.status);
            }

            return response.json();
        }).then(function(config) {
            document.getElementById('wifi_ssid').value = config.ssid || '';
            document.getElementById('wifi_password').value = config.password || '';

            document.getElementById('wifi_loading').style.display = 'none';
        }).catch(function(error) {
            document.getElementById('wifi_loading').innerHTML = 'Error: ' + error.message;
            console.error('Error: ', error);
        });
}
