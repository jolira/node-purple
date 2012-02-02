from os.path import exists
from shutil import rmtree

def set_options(opt):
  opt.tool_options('compiler_cxx')

def clean(cln):
  if exists('build'): rmtree('build')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  conf.check_cfg(package='purple', args='--cflags --libs', uselib_store='purple', mandatory=True)

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'purple'
  obj.source = [ 'src/purple.cc' ]
  obj.uselib = "purple"