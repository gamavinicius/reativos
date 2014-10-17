#Matéria Sistemas Reativos

##Tarefa-02
- Piscar o LED a cada 1 segundo
- Botão 1: Acelerar o piscapisca a cada pressionamento
- Botão 2: Desacelerar a cada pressionamento
- Botão 1+2 (em menos de 500ms): Parar

##Tarefa-03

###1. Implementar “event_driven.c”
  - Tratador para botões.
  - 2 timers.

###2. Reimplementar os exemplos com orientação a eventos:
  - Hello World: Input.
  - Tarefa 2.

##Exercício - “Reiniciar”
- Reiniciar o comportamento da “Tarefa 2” caso o Botão 1 seja pressionado por 5 segundos.


##Exercício - “Retomar”
- Retomar o movimento do retângulo após clicar novamente nele.

##Tarefa-Terra

###1. Faça um programa Terra em que o nó 11 envie periodicamente para a Estação Base (nó 1) o valor do seu sensor de temperatura.

###2. Assumindo uma numeração sequencial para os nós, faça um programa que:
  - O nó lê periodicamente seu valor de temperatura e envie para o nó com (NodeId-1).
  - O nó que receber uma mensagem de temperatura deve repassar para o nó (NodeId-1).
  - O nó 11 sempre deve repassar as mensagens para o nó 1 (BaseStation).