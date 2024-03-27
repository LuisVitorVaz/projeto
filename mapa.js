var map = L.map('map').setView([51.505, -0.09], 13);

// Camada de mapa de OpenStreetMap
L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

// Marcadores dos pontos de partida e destino
var startPoint = L.latLng(51.5, -0.09);
var endPoint = L.latLng(-20.1438, -44.1301);

L.marker(startPoint).addTo(map);
L.marker(endPoint).addTo(map);

// Configuração do controle de roteamento
var control = L.Routing.control({
    waypoints: [
        startPoint,
        endPoint
    ],
    routeWhileDragging: true // Atualizar a rota enquanto o usuário arrasta os marcadores
}).addTo(map);

// Evento para mostrar a distância da rota
control.on('routesfound', function (e) {
    var routes = e.routes;
    var summary = routes[0].summary;
    console.log('Distância: ' + summary.totalDistance + ' metros');
});
