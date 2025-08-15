#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

int main(int argc, char **argv) {
  Cmd cmd = {0};
  cc_setup(&cmd, 
    .flags        = CC_COMPILER|CC_INPUTS|CC_OUTPUT,
    .outpath      = "-",
//  .optional:.cc = "clang",
	.inpaths      = NOB_CC_JOIN_INPUTS("-"),
    .argc         = argc, 
    .argv         = argv);
  String_Builder sb={0};
  cmd_render(cmd,&sb);
  if(cmd.items&&sb.items&&sb.count>0){
  nob_log(NOB_INFO ,"[SUCCESS]  %.*s",(int)sb.count,sb.items);
  }else{
  nob_log(NOB_ERROR,"[FAIL]      ");
  }
  return 0;
}
