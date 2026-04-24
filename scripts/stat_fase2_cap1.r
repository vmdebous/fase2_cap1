library("readxl")
diagram <- read_excel("to_R.xlsx")

parte <- ifelse(as.integer(format(Sys.time(),"%H"))<12,"manha",ifelse(as.integer(format(Sys.time(),"%H"))<18,"tarde","noite"))
atual <- diagram[diagram$data == Sys.Date(),][[parte]]
condicional <- diagram[diagram[[parte]]==atual & diagram$data >= Sys.Date() - 5,]
dias_aux <- c("manha","tarde","noite")
prob_periodo1 <- prop.table(table(condicional[[dias_aux[dias_aux != parte][1]]]))
prob_periodo2 <- prop.table(table(condicional[[dias_aux[dias_aux != parte][2]]]))
previsao_periodo1 <- names(which.max(prob_periodo1))
previsao_periodo2 <- names(which.max(prob_periodo2))

library(openxlsx)
resultado <- data.frame(
  periodo = dias_aux[dias_aux != parte],
  prob = c(previsao_periodo1, previsao_periodo2)
)
write.xlsx(resultado, "from_R.xlsx")
