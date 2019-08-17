
from random import shuffle



def fade_in():
    disp = 0
    segments = [i for i in range(32)]
    shuffle(segments)
    print(segments)

    # all segments
    final_list = []

    for seg in segments:
        # print("-----------")
        disp |= 1<<seg
        # print("{:08x}".format(disp))
        
        for i in range(4):
            # final_list.append("
            # print(i)
            digit = disp & (0xff<<(3-i)*8)
            digit = digit >> (3-i)*8
            
            final_list.append("{:02x}".format(digit)) 

    lines = ""
    for i, a in enumerate(final_list):
        print(a,i)
        if i%4 == 0:
           print("ahi")
           lines +="\n"
        lines += "0x{},".format(a) 
        
    print(lines)
        
def fade_out(left_out_segments=None, start_segments=None):
    
    if start_segments is None:
        disp = 0xffffffff
        segments = [i for i in range(32)]
    else:
        disp = get_disp_from_segments(start_segments)
        segments = start_segments
    
    print(segments)

    if left_out_segments is not None:
        segments = list(set(segments) - set(left_out_segments))
    
    shuffle(segments)
    
    # all segments
    final_list = []

    for seg in segments:
        # print("-----------")
        disp^= 1<<seg
        # print("{:08x}".format(disp))
        
        for i in range(4):
            # final_list.append("
            # print(i)
            digit = disp & (0xff<<(3-i)*8)
            digit = digit >> (3-i)*8
            
            final_list.append("{:02x}".format(digit)) 

    lines = ""
    for i, a in enumerate(final_list):
        print(a,i)
        if i%4 == 0:
           print("ahi")
           lines +="\n"
        lines += "0x{},".format(a) 
        
    print(lines)

def get_disp_from_segments(segments_active):
    disp = 0x00000000
    for seg in segments_active:
        # print("-----------")
        disp^= 1<<seg
        # print("{:08x}".format(disp))
    return disp
    
sevSeg_4_digits_text_LODE = [29,28,27,16,17,18,19,20,21,12,11,10,9,14,0,5,4,3,6]

left_out_segments = sevSeg_4_digits_text_LODE  # LODE in 4 digits 7seg disp.
    
start_segments =    [29,28,27,21,20,19,18,17,16,12,11,10,14,9,0,5,4,3,6] # LODE in4 digits 7 segments.    

disp = get_disp_from_segments(sevSeg_4_digits_text_LODE)
# fade_in()   
fade_out(None, start_segments)    
 
# line = "".join(["0x{:02x},".format(i) for i in range(4)]) + "\n"
# print(line)