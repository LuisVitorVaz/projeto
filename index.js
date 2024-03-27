// Faz a ligação com o HTML
var dadosList = document.getElementById("dados-list");
var cont = 0;
var database = firebase.database();
var dadosRef = database.ref("dados"); // Substitua pelo ID correto do seu nó de dados



dadosRef.once("value")
  .then(function(snapshot) {
    if (!snapshot.exists()) {
      console.log("Nenhum dado encontrado.");
    } else {
      snapshot.forEach(function(childSnapshot) {
        // Acessa os valores diretamente no nível do childSnapshot
        var dataValor = childSnapshot.child("dado1").val();
        var horaValor = childSnapshot.child("dado2").val();
        var valorTexto = 100; // dados do campo valor
        console.log(dataValor);
        console.log(horaValor);

         // Chama a função para inserir os dados na tabela
        inserir_dados(dataValor, horaValor, valorTexto);

        const evento = new CustomEvent('atualizarDataHora', { detail: { dataValor, horaValor } });
        document.dispatchEvent(evento);

      });
    }
  })
  .catch(function(error) {
    console.error("Erro ao recuperar dados: " + error.message);
  });

// Escuta alterações contínuas no banco de dados
dadosRef.on("value", function(snapshot) {
  // Limpa a tabela antes de adicionar os novos dados
  dadosList.innerHTML = "";
  
  snapshot.forEach(function(childSnapshot) {
    // Acessa os valores diretamente no nível do childSnapshot
    var dataValor = childSnapshot.child("dado1").val();
    var horaValor = childSnapshot.child("dado2").val();
    var valorTexto = 100; // dados do campo valor
    
    // Chama a função para inserir os dados na tabela
    inserir_dados(dataValor, horaValor, valorTexto);

    
  });
});

function inserir_dados(dataValor, horaValor, valorTexto) {
  var newRow = dadosList.insertRow();
  // Adiciona células com os valores correspondentes
  newRow.insertCell(0).textContent = 1; // ID
  newRow.insertCell(1).textContent = dataValor;
  newRow.insertCell(2).textContent = horaValor;
  newRow.insertCell(3).textContent = valorTexto;
}
