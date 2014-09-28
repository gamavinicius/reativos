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
- LED para simular a cancela do estacionamento (Ainda não feito. Mensagem na saída da serial simula a cancela)

##Compilação e Execução
Para o funcionamento do projeto são necessárias as seguintes bibliotecas:
- VirtualWire (http://www.airspayce.com/mikem/arduino/VirtualWire/VirtualWire-1.27.zip)
- NewPing (http://playground.arduino.cc/Code/NewPing)

O arquivo 'receiver' deve ser instalado no Arduino que ficará na entrada do estacionamento.
O arquivo 'transmissor' deve ser instaldo no Arduino que ficará na saída do estacionamento.