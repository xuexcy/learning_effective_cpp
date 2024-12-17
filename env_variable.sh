this_script_dir=$(cd `dirname $0`; pwd)

# cmake 工作目录
export build_dir=${this_script_dir}/build
# 项目产出目录
export build_output_dir=$build_dir/output
# 编译产出的可执行文件存放目录
export build_output_bin_dir=$build_output_dir/bin

export stdout_dir=${this_script_dir}/stdout

export default_execute_file=11
