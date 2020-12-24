RTS <- function(FR, PAS, GCS, pesos = c(0.2908, 0.7326, 0.9368)) {
  score.FR = 0
  score.PAS = 0
  score.GCS = 0
  
  if (FR >= 29) {
    score.FR = 3
  } else if (FR >= 10) {
    score.FR = 4
  } else if (FR >= 6) {
    score.FR = 2
  } else if (FR >= 1) {
    score.FR = 1
  }
  
  if (PAS >= 89) {
    score.PAS = 4
  } else if (PAS >= 76) {
    score.PAS = 3
  } else if (PAS >= 50) {
    score.PAS = 2
  } else if (PAS >= 1) {
    score.PAS = 1
  }
  
  if (GCS >= 13) {
    score.GCS = 4
  } else if (GCS >= 9) {
    score.GCS = 3
  } else if (GCS >= 6) {
    score.GCS = 2
  } else if (GCS >= 4) {
    score.GCS = 1
  }    
  
  # Alterado por performance após profiling
  #
  # faixas.FR = rbind(c(29, 3),
  #                   c(10, 4),
  #                   c(6, 2),
  #                   c(1, 1),
  #                   c(0, 0))
  # 
  # score.FR = faixas.FR[FR >= faixas.FR[, 1], , drop = FALSE][1, 2]
  # 
  # faixas.PAS = rbind(c(89, 4),
  #                    c(76, 3),
  #                    c(50, 2),
  #                    c(1, 1),
  #                    c(0, 0))
  # 
  # score.PAS = faixas.PAS[PAS >= faixas.PAS[, 1], , drop = FALSE][1, 2]
  # 
  # faixas.GCS = rbind(c(13, 4),
  #                    c(9, 3),
  #                    c(6, 2),
  #                    c(4, 1),
  #                    c(3, 0))
  # 
  # score.GCS = faixas.GCS[GCS >= faixas.GCS[, 1], , drop = FALSE][1, 2]
  
  return(score.FR * pesos[1] + score.PAS * pesos[2] + score.GCS * pesos[3])
}

ISS <- function(AIS = c(1, 1, 1)) {
  if (any((AIS == 6))) {
    ret = 75
  } else {
    ret = sum(AIS ^ 2)
  }
  
  return(ret)
}

TRISS.precalc <- function(RTS, ISS, idade, blunt = TRUE, limiteIdade = 55,
                          b = rbind(c(-1.2470, 0.9544, -0.0768, -1.9052),
                                    c(-0.6029, 1.1430, -0.1516, -2.6676))) {
  if (idade >= limiteIdade) {
    age = 1
  } else {
    age = 0
  }
  
  if (blunt) {
    i = 1
  } else {
    i = 2
  }
  
  # Alterado por performance apos profiling
  #
  # age =  ifelse(idade >= limiteIdade, 1, 0)
  # i = ifelse(blunt, 1, 2)
  
  B = b[i, 1] + b[i, 2] * RTS + b[i, 3] * ISS + b[i, 4] * age
  
  return(1 / (1 + exp(-B)))
}

TRISS <- function(FR, PAS, GCS, ISS, idade, blunt = TRUE, limiteIdade = 55,
                  pesos.RTS = c(0.2908, 0.7326, 0.9368),
                  b = rbind(c(-1.2470, 0.9544, -0.0768, -1.9052),
                            c(-0.6029, 1.1430, -0.1516, -2.6676))) {
  RTS = RTS(FR, PAS, GCS, pesos.RTS)
  
  return(TRISS.precalc(RTS, ISS, idade, blunt, limiteIdade, b))
}
