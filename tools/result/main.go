package main

import (
    "errors"
    "log"
    "os"
    "path/filepath"
    "regexp"
    "runtime"
    "sort"
    "strings"
)

type Result struct {
    name      string
    category  string
    nameLower string
    file      string
    parent    string
    results   []string
}

var (
    reAddEXE = regexp.MustCompile(`add_executable\((.+?)\s(.*?)\s*\)`)
    results  = map[string]Result{}
)

func BaseName(s string) string {
    n := strings.LastIndexByte(s, '.')
    if n == -1 {
        return s
    }
    return s[:n]
}

func Filename() (string, error) {
    _, filename, _, ok := runtime.Caller(1)
    if !ok {
        return "", errors.New("unable to get the current filename")
    }
    return filename, nil
}

func Dirname() (string, error) {
    filename, err := Filename()
    if err != nil {
        return "", err
    }
    return filepath.Dir(filename), nil
}

func processCMake(value string) {
    data, err := os.ReadFile(value)
    if err != nil {
        log.Println(err)
    }
    res := reAddEXE.FindAllStringSubmatch(string(data), -1)
    category := ""
    parent := ""
    if start := strings.LastIndex(value, "src"); start > 0 {
        for i := start + 4; i < len(value); i++ {
            if value[i] == '/' || value[i] == '\\' {
                category = value[start+4 : i]
                break
            }
        }
    }
    if start := strings.LastIndex(value, "src"); start > 0 {
        for i := start; i < len(value); i++ {
            if value[i] == '/' || value[i] == '\\' {
                parent = value[start : i+1]
            }
        }
        parent = strings.ReplaceAll(parent, "\\", "/")
    }

    for _, v := range res {
        files := strings.Split(v[2], " ")
        if len(files) > 0 {
            results[v[1]] = Result{
                name:      v[1],
                category:  category,
                nameLower: strings.ToLower(v[1]),
                parent:    parent,
                file:      files[0],
            }
        }
    }
}

func ProcessDir(dir string) {
    if info, err := os.Stat(dir); err != nil || !info.IsDir() {
        log.Fatal(dir + "is not directory")
        return
    }
    ptdir := dir + "/*"

    files, _ := filepath.Glob(ptdir)
    for _, value := range files {
        if info, err := os.Stat(value); err == nil && info.IsDir() {
            ProcessDir(value)
            continue
        }
        if strings.HasSuffix(value, "CMakeLists.txt") {
            processCMake(value)
        }
    }
}

func ProcessResultFile(parent, file string) {
    base := filepath.Base(file)
    baseName := BaseName(base)
    if val, ok := results[baseName]; ok {
        resultUrl := "results/" + parent + "/" + base
        result := parent + "/" + base
        val.results = append(val.results, "["+result+"]"+"("+resultUrl+")")
        results[baseName] = val
    }
}

func ProcessDirResults(dir string) {
    if info, err := os.Stat(dir); err != nil || !info.IsDir() {
        log.Fatal(dir + "is not directory")
        return
    }
    ptDir := dir + "/*"
    base := filepath.Base(dir)
    baseName := BaseName(base)
    files, _ := filepath.Glob(ptDir)
    for _, value := range files {
        if info, err := os.Stat(value); err == nil && info.IsDir() {
            ProcessDirResults(value)
            continue
        }
        ProcessResultFile(baseName, value)
    }
}
func WriteResult() {
    slice := make([]Result, 0, len(results))
    for _, v := range results {
        slice = append(slice, v)
    }
    sort.Slice(slice, func(i, j int) bool {
        switch {
        case slice[i].category < slice[j].category:
            return true
        case slice[i].category > slice[j].category:
            return false
        default:
            return slice[i].nameLower < slice[j].nameLower
        }
    })
    sb := strings.Builder{}
    for _, v := range slice {
        if len(v.results) > 0 {
            res := strings.Join(v.results, "<br/>")
            bFile := filepath.Base(v.file)
            rFile := "[" + bFile + "](" + v.parent + v.file + ")"
            data := append([]string{}, "", v.category, v.name, rFile, res, "")
            rData := strings.Join(data, "|")
            sb.WriteString(rData)
            sb.WriteString("\n")
        }
    }
    os.WriteFile("results.txt", []byte(sb.String()), os.ModePerm)
}

func main() {
    dir, _ := Dirname()
    srcDir := dir + "/../../src/"
    ProcessDir(srcDir)
    resultsDir := dir + "/../../results/"
    ProcessDirResults(resultsDir)
    WriteResult()
}
