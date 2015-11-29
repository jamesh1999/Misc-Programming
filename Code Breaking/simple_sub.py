from simplesubstitution import SimpleSubstitution as SimpleSub
import random,re
from fitness import fitness

fitness = fitness('bigrams.txt')

CTEXT='''

LMIZ XPQT, 
 
PWE KWCTL Q XIAA CX BPM WXXWZBCVQBG BW EWZS WV BPQA? Q PIL I KZIKS IB IVWBPMZ WN BPM VWBMA GWC NWCVL QV UWVBUIZBZM. QB TWWSA TQSM BPM WTLMAB QBMU QV BPM XIKSMB IVL Q BPWCOPB QB UQOPB JM I OWWL XTIKM BW ABIZB. QB UISMA AWUM AMVAM WN BPM JCKPMVEITL ZMNMZMVKM QV BPM TIBMZ VWBM GWC AMVB. 
 
QB LQLV'B MFXTIQV BPM XIZQA TQVS AW Q AMVB I BMIU QVBW BPIB VMQOPJWCZPWWL BW OIBPMZ QVBMT JCB BPMG LQLV'B KWUM CX EQBP DMZG UCKP. EM LQL OMB WVM ZMXWZB BPIB BPM PWCAM PIL JMMV WEVML JG I OMZUIV NIUQTG JMNWZM BPM EIZ, JCB BPIB QB PIL JMMV BISMV WDMZ JG IV AA WNNQKMZ QV VQVMBMMV NWZBG WVM. ACZMBM ZMKWZLA ACOOMAB BPIB BPM NIUQTG KIUM NZWU EMQUIZ QV ICOCAB VQVMBMMV BPQZBG AMDMV, EPQKP QA ACOOMABQDM OQDMV BPM BQUQVO IVL BPM OMWOZIXPG, AW Q PIDM AMVB BPM BMIU BW JCKPMVEITL BW AMM EPIB BPMG KIV NQVL. 
 
Q IU RCUXQVO BW KWVKTCAQWVA PMZM, JCB BPM XWZBZIQB AIZIP UMVBQWVA PIA BW JM BPM UWVI TQAI. Q PIDM JMMV BZGQVO BW OMB IKKMAA BW QB, JCB BPM NZMVKP ICBPWZQBQMA IZM AXWWSML. BPM BPMNB WN BPM XIQVBQVO JG XMZCOOQI JIKS QV VQVMBMMV MTMDMV PIA UILM BPMU DMZG AMVAQBQDM. BPM VIUM KPICLZWV EIA UMVBQWVML UWZM BPIV WVKM, IVL Q VMML AWUM BQUM BW TWWS QVBW BPM PQABWZG. 
 
QN GWC PIDM IVG QVNTCMVKM IB ITT IB BPM UCAMCU Q BPQVS EM VMML I XZWXMZ MFIUQVIBQWV WN BPM XIQVBQVO, IVL Q VMML BW SVWE EPIB PIXXMVML BW QB LCZQVO BPM EIZ. 
 
ITT BPM JMAB, 
 
PIZZG

'''
CTEXT = re.sub('[^A-Z]','',CTEXT.upper())

maxkey = list('ABCDEFGHIJKLMNOPQRSTUVWXYZ')
maxscore = -99e9
parentscore,parentkey = maxscore,maxkey[:]

i = 0

while True:

    i += 1

    random.shuffle(parentkey)

    deciphered = SimpleSub(parentkey).decipher(CTEXT)
    parent_score = fitness.score(deciphered)

    count = 0
    while count < 500:
        a = random.randint(0,25)
        b = random.randint(0,25)

        child = parentkey[:]
        child[a],child[b] = child[b],child[a]
        
        deciphered = SimpleSub(child).decipher(CTEXT)
        score = fitness.score(deciphered)
       
        if score > parent_score:
            parent_score = score
            parentkey = child[:]
            count = 0

        count +=1
    
    if parent_score>maxscore:
        maxscore,maxkey = parent_score,parentkey[:]
        print('\nBest score so far:',maxscore,'on iteration'+" "+str(i))
        ss = SimpleSub(maxkey)
        print('\tBest key:\n\t '+''.join(maxkey))
        print('\tPlaintext:\n\t '+ss.decipher(CTEXT))

