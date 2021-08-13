
NPROC = `nproc`.to_i

class OS
  def self.linux?
    RUBY_PLATFORM != 'x86_64-darwin19'
  end
  def self.macOS?
    RUBY_PLATFORM == 'x86_64-darwin19'
  end
end

def runLinux
  mkdir_p 'cmake'
  mkdir_p 'cmake/linux'
  Dir.chdir 'cmake/linux'
  sh "cmake -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=Release -Wno-dev ../.. && cmake --build . --config Release -- -j #{NPROC}"
  Dir.chdir '../..'
end

def runMacOS
  mkdir_p 'cmake'
  mkdir_p 'cmake/xcode'
  Dir.chdir 'cmake/xcode'
  sh "cmake -GXcode -Wno-dev ../.. && cmake --build . --config Release  -- -j #{NPROC}"
  Dir.chdir '../..'
  sh 'cp build/Release/* build'
end

# directory 'build'

task :default => :build

desc 'Build aai and libs'
task :build do
  runLinux if OS.linux?
  runMacOS if OS.macOS?
end

task :clean do
  rm_rf 'build'
end

task :clean_all do
  Rake::Task["clean"].invoke
  rm_rf 'cmake'
end

task :noop do
end
