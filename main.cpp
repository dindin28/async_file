#include <unistd.h> // fork

#include <fstream>  // std::ofstream, std::ifstream
#include <sstream>  // std::stringstream
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <chrono>   // std::chrono...

#define INPUT_FILE "input_sample.txt"

void FileCheckSum(std::string file_path,
                  std::chrono::time_point<std::chrono::high_resolution_clock> start)
{
  unsigned long long sum = 0;
  std::ifstream file_input(INPUT_FILE);
  if (file_input.is_open() == false) // Check if file opened
  {
    std::cout << "Could not open input_file" << std::endl;
  }
  else
  {
    // Read file to string stream
    std::stringstream sstream;
    sstream << file_input.rdbuf();
    file_input.close();

    // Add file check sum to string stream
    std::string string(sstream.str());
    for (int iter : string)
    { sum += iter; }
    sstream << "[" << sum << "]";

    // Add process duration to string stream
    auto dur_time = std::chrono::system_clock::now() - start;
    sstream << " " << std::chrono::duration_cast<std::chrono::microseconds>(dur_time).count() << " microseconds";

    // Create and fill output file
    std::ofstream file_output(file_path);
    file_output << sstream.rdbuf();
    file_output.close();
  }
}

int main()
{
  // Function in thread
  std::thread(FileCheckSum, "output_thread_sample.txt", std::chrono::high_resolution_clock::now()).join();

  // Function in child and parent
  pid_t pid = fork();
  if(pid == 0) 
  { FileCheckSum("output_child_sample.txt", std::chrono::high_resolution_clock::now()); } // In child process
  else
  { FileCheckSum("output_parent_sample.txt", std::chrono::high_resolution_clock::now()); } // In parent process

  return 0;
}