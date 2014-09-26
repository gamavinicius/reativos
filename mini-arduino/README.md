#Controle de estacionamento

##Descrição
O projeto visa controlar a entrada e saída de veículos de um estacionamento com um número determinado de vagas.

##Como funciona
- Existe um número definido de quantas vagas há no estacionamento (independente do tamanho do veículo)
- É instalado um Arduino em cada entrada e saída
- Se a diferença entre veículos que entraram e que sairam for igual ao número de vagas do estacionamento, então não é permitido mais a entrada até a liberação de pelo menos uma vaga

##Componentes:
- Dois Arduinos
- Dois sensores de distâncias
- Transmissor e receptor de Rádio Frequência
- LED para simular a cancela do estacionamento