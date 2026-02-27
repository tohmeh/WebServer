/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mal-naki <mal-naki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:01:24 by mal-naki          #+#    #+#             */
/*   Updated: 2025/10/14 20:01:24 by mal-naki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "Utils.hpp"

// *Constructor initializes CGI with script path and environment variables
CGI::CGI(const std::string &path, const std::string &interpreter, const std::map<std::string, std::string> &env) : _path(path), _interpreter(interpreter), _env(env) {};

// *Destructor cleans up CGI object
CGI::~CGI() {};

/*
 *Converts the environment variables map into
 *a NULL-terminated array of C-strings for execve.
 */
std::vector<char *> CGI::initEnv() const
{
    std::vector<char *> envp;

    std::map<std::string, std::string>::const_iterator it = _env.begin();
    while (it != _env.end())
    {
        std::string entry = it->first + "=" + it->second;
        char *envStr = new char[entry.length() + 1];
        std::strcpy(envStr, entry.c_str());
        envp.push_back(envStr);
        ++it;
    }
    envp.push_back(NULL);
    return (envp);
}

/*
 *Executes a CGI script sends request body to the child process, reads its output,
 *waits for the child to finish, and returns the response body without headers.
 */
std::string CGI::execute(const std::string &body)
{
    int inputPipe[2];
    int outputPipe[2];

    if (pipe(inputPipe) == -1 || pipe(outputPipe) == -1)
        throw std::runtime_error("pipe -> failed");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork -> failed");

    //? Child
    //? [0] is for reading
    //? [1] is for writing
    //? inputPipe → Parent writes → Child reads (for request data)
    //? outputPipe → Child writes → Parent reads (for response data)
    if (pid == 0)
    {
        dup2(inputPipe[0], STDIN_FILENO);
        dup2(outputPipe[1], STDOUT_FILENO);
        close(inputPipe[1]);
        close(outputPipe[0]);

        std::vector<char *> envp = initEnv();

        char *av[3];
        av[0] = new char[_interpreter.length() + 1];
        std::strcpy(av[0], _interpreter.c_str());
        av[1] = new char[_path.length() + 1];
        std::strcpy(av[1], _path.c_str());
        av[2] = NULL;

        execve(av[0], av, &envp[0]);
        ft_error("execve -> failed");

        delete[] av[0];
        delete[] av[1];
        for (size_t i = 0; i < envp.size() && envp[i] != NULL; ++i)
            delete[] envp[i];
        throw std::runtime_error("execve failed");
    }
    //? Parent
    close(inputPipe[0]);
    close(outputPipe[1]);

    //? Send the POST request body to the child process
    ssize_t totalWritten = 0;
    ssize_t toWrite = body.size();
    const char *buf = body.c_str();
    while (totalWritten < toWrite)
    {
        ssize_t written = write(inputPipe[1], buf + totalWritten, toWrite - totalWritten);
        if (written <= 0)
            break;
        totalWritten += written;
    }
    close(inputPipe[1]);

    //? Read the response from the child process
    char buffer[4096];
    std::string res;
    ssize_t bytesRead;
    while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer))) > 0)
        res.append(buffer, bytesRead);
    close(outputPipe[0]);

    int status;
    waitpid(pid, &status, 0);

    size_t headerEnd = res.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        headerEnd = res.find("\n\n");
    if (headerEnd != std::string::npos)
        return res.substr(headerEnd + ((res[headerEnd] == '\r') ? 4 : 2));

    return (res);
}

//! Mohamad AL-Nakib | Webserve