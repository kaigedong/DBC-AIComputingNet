from matrix.ttypes import *
class inner_header(object):
    def __init__(self):
        self.msg_name = "unknown_message"
        self.msg_priority = 0
        self.src_id = 0
        self.dst_id = 0

class inner_message(object):
    header = inner_header()
    msg_content = object()
    msg_head=object()
