var cont = 0;
var dataValor;
var horaValor;
var lat;
var long;
var dadoAnterior1 = null; // Variável global para armazenar o dado anterior
var dadoAnterior2 = null; // Variável global para armazenar o dado anterior
var polyline; // Variável para armazenar a linha poligonal
var routingControl; // Variável para armazenar o controle de rota
var waypoints = []; // Array para armazenar os pontos da rota
var coordenadas_iniciais_lat, coordenadas_iniciais_lon;
var dadosList = document.getElementById("data-box");
var database = firebase.database();
var dadosRef = database.ref("dados"); // Substitua pelo ID correto do seu nó de dados
export var alertCircleRed; // Variável para o círculo vermelho
export let circulosdealerta = []; // Lista para armazenar os pontos adicionados
export let circulosAdicionados = []; // Lista para armazenar os pontos adicionados
export let trajetoria = []; // lista contendo os a ligacao entre os pontos
export let linhapontos = []; // guarda todos os pontos recebidos entre as linhas 
let alertCircle; // Declare alertCircle como uma variável global
export let barcoprimeiro // contem os dados do barco 1

var map; // Variável para armazenar o mapa

// dadosRef.once("value").then(function(snapshot) {
//     // Verifica se há dados no snapshot
//     if (snapshot.exists()) {
//         // Se existir, você pode iterar sobre eles ou fazer qualquer outra operação necessária
//         console.log("Há dados no banco de dados.");
//         iniciarMapa(ajuda1, ajuda2, ajuda1, ajuda2);
//     } else {
//         console.log("Não há dados no banco de dados.");
//     }
// }).catch(function(error) {
//     console.error("Erro ao acessar o banco de dados:", error);
// });
// Evento "child_added" para lidar com novos dados
dadosRef.on("child_added", function(childSnapshot) {

     // Acessa os valores diretamente no nível do childSnapshot
    dataValor = childSnapshot.child("dado1").val();
    horaValor = childSnapshot.child("dado2").val();
    console.log("Novo dado adicionado:");
    console.log(dataValor);
    console.log(horaValor);
   
    // Exemplo de uso
    const coordenadas1 = converterParaCoordenadas(dataValor, horaValor);
    console.log("aqui esta \n");
    console.log("Latitude:", coordenadas1.latitude);
    console.log("Longitude:", coordenadas1.longitude);

    // recupera a lat e longitude inicialmente
    if (cont == 0) {
        coordenadas_iniciais_lat = coordenadas1.latitude;
        coordenadas_iniciais_lon = coordenadas1.longitude;
        console.log("dados iniciais");
        cont = 50;
    }

    // Verifica se o mapa já foi inicializado
    if (!map) {
        iniciarMapa(coordenadas_iniciais_lat, coordenadas_iniciais_lon, coordenadas1.latitude, coordenadas1.longitude);
    }
    waypoints.push(L.latLng(coordenadas1.latitude, coordenadas1.longitude));

    adicionarCirculo(coordenadas1.latitude, coordenadas1.longitude); // Adiciona o círculo no novo ponto

    processarNovoDado(coordenadas1.latitude, coordenadas1.longitude); // Adiciona a nova coordenada à linha existente
  
    // Atualiza o HTML com os novos valores
    atualizarHTML(coordenadas1.latitude, coordenadas1.longitude);
});
// teste1 e teste2  contem os dados apos o primeiro dado postado no banco no caso e onde sera recebido lat e long apos o ponto incial
function iniciarMapa(lat, lon, teste1, teste2) {
    console.log("dados dentro do mapa :", teste1);
    console.log("dados dentro do mapa : ", teste2);
    // Coordenadas do Ponto A - Porto Alegre, Brasil
    const coordTaxi = [lat, lon];
    toggleCircle(); // Inicia a alternância de visibilidade do círculo
    
    // const coordTaxi = [-30.0346, -51.2177];
    // Coordenadas do Ponto B - Usuário (coordenadas fictícias)
    const coordUser = [teste1, teste2];

    // Inicia o mapa com coordenadas do ponto A
    map = L.map('map').setView(coordTaxi, 13);

    // L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png').addTo(map);

     // Definir camadas base
     var osmLayer = L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', { attribution: 'OpenStreetMap' });
     var osmHotLayer = L.tileLayer('http://{s}.tile.openstreetmap.fr/hot/{z}/{x}/{y}.png', { attribution: 'OpenStreetMap.Hot' });
 
    // Adicionar camadas base ao controle de camadas
    var baseLayers = {
        "OSM": osmLayer,
        "OSM Hot": osmHotLayer
        };
    L.control.layers(baseLayers).addTo(map);

    // var overlays = {
    //     "Rota": routingControl
    // };

    // L.control.layers(null, overlays).addTo(map);
     // Adicionar camada base padrão
     osmLayer.addTo(map);

    // Adicionar camada base padrão
    osmLayer.addTo(map);
   // circulo da funcao de alerta 
   alertCircleRed = L.circle([-30.165924, -51.403555], {
        color: 'red', // Cor do círculo
        fillColor: 'red', // Cor de preenchimento do círculo
        fillOpacity: 0.5, // Opacidade do preenchimento do círculo
        radius: 10 // Raio do círculo
    }).addTo(map);
    circulosdealerta.push(alertCircleRed); // Adiciona o ponto à lista
  

    // Personaliza o ponto no mapa com imagem do táxi.
    const taxiIcon = L.icon({
        className: "taxi-pointers",
        iconUrl: 'imagens/barcosemfundo.png',
        iconSize: [45, 45]
    });
    barcoprimeiro=taxiIcon;
    var taxiMarker = L.marker(coordTaxi, { icon: taxiIcon }).addTo(map);
    taxiMarker.bindPopup('inicio');
    // Identifica a melhor rota para iniciar a viagem.
    L.Routing.control({
        waypoints: [
            L.latLng(coordTaxi[0], coordTaxi[1]),
            L.latLng(coordUser[0], coordUser[1])
        ]
    }).on('routesfound', function(e) {
        const latlng = e.routes[0].coordinates;

        // Simula o táxi enviando a localização para o APP.
        latlng.forEach(function(coord, index) {
            setTimeout(function() {
                taxiMarker.setLatLng([coord.lat, coord.lng]);

                // Identifica o final da viagem.
                if (coord.lat === coordUser[0] && coord.lng === coordUser[1]) {
                    alert('Seu táxi acabou de chegar!');
                }
            }, 1000 * index);
        });
    }).addTo(map);

    // Inicializa a linha poligonal com a primeira coordenada
    iniciarLinha(lat, lon);
}

function atualizarRota() {
    routingControl.setWaypoints(waypoints); // Atualiza os waypoints da rota
}

function atualizarHTML(dataValor, horaValor) {
    // Atualiza o HTML com os valores correspondentes
    document.getElementById("latitude").textContent = dataValor;
    document.getElementById("longitude").textContent = horaValor;
}

function converterParaCoordenadas(latitude, longitude) {
    lat = latitude / 1e6; // Dividir por 1 milhão para obter a parte decimal correta
    long = longitude / 1e6;
    return { latitude: lat, longitude: long };
}

export function adicionarCirculo(lat, lon) {
    // Cria um círculo com raio de 3 metros (alterado de 1 para 3 para melhor visualização)
    alertCircle = L.circle([lat, lon], {
        color: 'green', // Cor da linha do círculo
        fillColor: 'green', // Cor de preenchimento do círculo
        fillOpacity: 0.5, // Opacidade do preenchimento
        radius: 3 // Raio do círculo em metros
    }).addTo(map); // Adiciona o círculo ao mapa
    circulosAdicionados.push(alertCircle); // Adiciona o ponto à lista
    adicionarCoordenadaNaLinha(lat, lon);
}

export function iniciarLinha(lat, lon) {
    // Inicializa a linha poligonal com a primeira coordenada
    polyline = L.polyline([[lat, lon]], { color: 'blue' }).addTo(map);
    trajetoria.push(polyline);
}

function adicionarCoordenadaNaLinha(lat, lon) {
    // Adiciona a nova coordenada à linha poligonal
    polyline.addLatLng([lat, lon]);
}

function processarNovoDado(novo1, novo2) {
    // Verifica se há um dado anterior
    if (dadoAnterior1 !== null && dadoAnterior2 !== null) {
        // Cria uma linha entre o dado anterior e o novo dado
        criarLinhaEntrePontos(dadoAnterior1, dadoAnterior2, novo1, novo2);
    }
    
    // Atualiza os dados anteriores com os novos dados
    dadoAnterior1 = novo1;
    dadoAnterior2 = novo2;
}

export function criarLinhaEntrePontos(lat1, lon1, lat2, lon2) {
    // Calcula a distância entre os pontos usando a função distanceTo() do Leaflet
    const distance = L.latLng(lat1, lon1).distanceTo([lat2, lon2]);
    linhapontos.push(distance);
}
// Função para alternar a visibilidade do círculo

export function toggleCircle(intervalo) {
       
    var isVisible = true;
    setInterval(function() {
        if (isVisible) {
            alertCircleRed.setStyle({ fillOpacity: 0 }); // Torna o círculo invisível
        } else {
            alertCircleRed.setStyle({ fillOpacity: 0.5 }); // Torna o círculo visível
        }

        isVisible = !isVisible; // Inverte o estado de visibilidade
    }, 500); // Alterna a cada 500 milissegundos (meio segundo)
    }
 