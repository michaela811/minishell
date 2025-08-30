# minishell (42cursus)

> **Passing the Gate.** Minishell is widely seen as the *main threshold* of 42: the first big systems project where you must design, test, and debug a real UNIX program under strict constraints (no leaks, precise signals, exact exit codes, brittle specs). Many students struggle not because it’s “hard C,” but because **small behavioral details** matter and are tested.
> Written in a group together with https://github.com/grignetta

---

## TL;DR

* **Goal:** Re‑implement a tiny Bash‑like shell.
* **You must get right:** parsing, quoting, environment/expansion, redirections, pipelines, builtins, **signals** (Ctrl‑C/D/), **heredocs**, and **exit codes**.
* **Why this project matters:** It forces you to build a clean architecture (lexer → parser → executor) and to develop production habits: test harnesses, robust error handling, and clean resource management.

---

## Features (Mandatory Scope)

* Prompt loop using **readline** with history.
* **Lexer/Parser** producing a parse tree (or equivalent) that supports:

  * Words, operators (`|`, `<`, `>`, `<<`, `>>`), and correct **precedence**.
  * **Quotes**: single quotes = literal, double quotes = expand `$` & `$?`.
* **Expansion**: `$VAR`, `$?` (last exit status), rules with quotes.
* **Redirections**: `>`, `>>`, `<`, `<<` (heredoc with optional expansion).
* **Pipelines**: `cmd1 | cmd2 | ...` with correct FD wiring.
* **Builtins** implemented in‑process when appropriate:

  * `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
* **Signals** & termios behavior matching Bash‐like expectations.
* **No memory leaks** over long interactive sessions; correct cleanup for all code paths.

---

## Architecture

```
include/           # headers
src/
  main/            # entry, init, signals, errors
  env/             # env storage & manipulation
  lexer/           # tokenization
  parser/          # grammar -> AST
  expand/          # quotes & $ expansion
  exec/            # executor, redirs, pipes
  heredoc/         # heredoc management
  builtins/        # builtins implementations
  path/            # PATH resolution
  free/            # cleanup helpers
printf_fd/         # libprintf_fd.a (ft_printf_fd & helpers)
libft/             # submodule (libft.a)
tools/suppress/    # sanitizer suppression files (readline, etc.)
```

* **Executor rule of thumb:**

  * **Single builtin with redirs** → run **in parent** (so it can change shell state).
  * **Pipelines / external commands** → run in **children**.

---

## Build & Run

```sh
# clone with submodules
git clone --recursive https://github.com/michaela811/minishell.git minishell
cd minishell

# build
make

# run
./minishell
```

### Make targets

* `make` – build `minishell`, `libft.a`, and `libprintf_fd.a`.
* `make clean|fclean|re` – standard hygiene.

> If you forgot submodules: `git submodule update --init --recursive`.

---

## The Controls You Must Pass (Signals & TTY behavior)

Getting **Ctrl keys** right is a common failure point. The parent shell and its children must behave differently.

### Ctrl‑C (SIGINT)

* **At prompt (no child running):**

  * Effect: print a newline, **redisplay prompt**, don’t exit.
  * **Exit status** becomes `130` for `$?`.
  * Implementation: handle SIGINT in the parent (reset current line via readline hooks), do **not** terminate the shell.
* **While a child runs:**

  * The child receives SIGINT and terminates; parent prints newline and sets status `130`.

### Ctrl‑\ (SIGQUIT)

* **At prompt (no child):** ignore (no message, no exit).
* **While a child runs:** if the child doesn’t ignore SIGQUIT, it ends with core‑dump semantics; many testers check for the message `Quit: 3` and **status `131`**.

### Ctrl‑D (EOF)

* **At empty prompt:** exit cleanly; print `exit` and return last status.
* **In heredoc:** treated as end of file for the heredoc **only**.

> **Summary table**
>
> | Key     | At prompt (parent) | While child runs | `$?`  |
> | ------- | ------------------ | ---------------- | ----- |
> | Ctrl‑C  | newline + prompt   | kills child      | 130   |
> | Ctrl‑\  | ignored            | “Quit: 3”        | 131   |
> | Ctrl‑D  | exit shell         | (n/a)            | last  |
> | Ctrl‑Z  | ignored            | default in child | (n/a) |

---

## Heredoc Rules That Are Tested

* **Delimiter quoting controls expansion**:

  * Unquoted delimiter → expand `$VAR` and `$?` inside heredoc.
  * Quoted delimiter → **no expansion**.
    
* **Signals inside heredoc**:

  * `Ctrl‑C` must **abort** the heredoc, clean temp files, and set status `130`.
* Use a temp file or pipe; ensure **FD hygiene** in pipelines.

---

## Exit Codes You’ll Be Graded On

* `0` – success.
* `126` – found but **not executable** (permissions / directory exec attempt).
* `127` – **command not found**.
* `130` – terminated by **SIGINT**.
* `131` – terminated by **SIGQUIT**.

Also ensure `exit` builtin accepts: `exit`, `exit <n>`, error on too many args, numeric parsing as Bash.

---

## Redirections & Pipelines – Common Edge Cases

* Redirect errors (e.g., `cat < nofile`) must not create processes needlessly; print error and set correct status.
* Mix of redirs & pipelines: last command’s exit status propagates to `$?`.
* `cd` with redirs (e.g., `cd > file`) must still run in parent when possible; handle errors before state changes.

---

## Expansion & Quotes – Quick Rules

* Single quotes `'...'` → literal.
* Double quotes `"..."` → expand `$VAR` and `$?`; keep spaces as literal characters.
* Unquoted words are subject to splitting by the parser **before** expansion is resolved into argv; design your lexer/parser so that quotes affect tokenization, not post‑processing hacks.

---

## Memory Hygiene & Suppression Files (about “suppress/susspend”)

`readline` and some system libs can report **false‑positive leaks** under ASan/LSan or Valgrind. Use **suppression files** to focus on *your* leaks.

* Suppression files live in `tools/suppress/`:

  * `suppress_lnx`, `suppress_mac`, `m_suppress`, etc.
* **Run with Address/Leak sanitizer** (Linux example):

  ```sh
  ASAN_OPTIONS="detect_leaks=1:halt_on_error=1:suppressions=tools/suppress/suppress_lnx" \
  LSAN_OPTIONS="suppressions=tools/suppress/suppress_lnx" \
  ./minishell
  ```
* **On macOS** (different env vars):

  ```sh
  export ASAN_OPTIONS="detect_leaks=1:suppressions=tools/suppress/suppress_mac"
  export LSAN_OPTIONS="suppressions=tools/suppress/suppress_mac"
  ./minishell
  ```
* **Why use suppressions?**

  * School testers expect you to eliminate *your* leaks; third‑party library internals (e.g., `readline`) aren’t your responsibility.

> Still use `valgrind`/ASan on targeted unit tests (builtins, parser failures, heredoc aborts) to prove no leaks on every path.

---

## Step‑by‑Step Plan to Pass

1. **REPL skeleton** with readline, history, and clean Ctrl‑C/D behavior at prompt.
2. **Lexer** producing tokens (words, ops). Handle quotes during tokenization.
3. **Parser** building an AST/commands list; validate grammar, produce friendly errors.
4. **Executor skeleton** (no pipes/redirs yet): spawn external cmd, run builtins in parent.
5. **PATH resolution** + clear error messages (126/127).
6. **Redirections** (>, >>, <) with FD lifecycle and error propagation.
7. **Pipelines**: connect FDs, collect last status.
8. **Expansion**: `$VAR`, `$?`, quoting rules.
9. **Builtins** complete & consistent (env table, export/unset behavior).
10. **Heredoc** with delimiter rules + SIGINT abort.
11. **Signals in children**: restore defaults; print correct messages.
12. **Cleanups**: sweeping frees on all early‑return/error paths.
13. **Test matrix**: scripts to cover signals, pipes, redirs, heredoc, big env, weird filenames.

---

## Testing

#### Manual tests (quick checklist)
- **Ctrl‑C at prompt:**
  1. Run `./minishell`.
  2. Press **Ctrl‑C** once: you should see a newline + fresh prompt.
  3. Type `echo $?` → expect `130`.
- **Ctrl‑C while child runs:**
  1. Type `sleep 5` and press **Enter**.
  2. Press **Ctrl‑C** → the sleep ends immediately.
  3. Type `echo $?` → expect `130`.
- **Ctrl‑\ at prompt:**
  1. Press **Ctrl‑\** → nothing should print, prompt remains.
  2. `echo ok` → prints `ok`. `$?` unchanged.
- **Ctrl‑\ while child runs:**
  1. Run `cat` and press **Enter** (it waits on stdin).
  2. Press **Ctrl‑\** → you should see `Quit: 3` and return to prompt.
  3. `echo $?` → expect `131`.
- **Ctrl‑D at prompt (EOF):**
  1. From a fresh `./minishell`, press **Ctrl‑D** on an empty line.
  2. Shell prints `exit` and terminates.
  3. In your outer shell, run `echo $?` → expect previous minishell status (often `0`).
- **Ctrl‑Z (suspend):**
  - **At prompt:** press **Ctrl‑Z** → nothing should happen (ignored).
  - **While child runs:** run `cat`, then **Ctrl‑Z** → the child stops (`Stopped`) while minishell stays responsive. Since minishell has no job control, manually `kill -SIGCONT <pid>` or `kill -9 <pid>` from another terminal if needed.

#### Pipeline interruption
- Run: `sleep 5 | cat` → press **Ctrl‑C**.
- Expect all children to terminate and `$?` to reflect the **last command** (typically `130`).

#### Heredoc specifics
- **Unquoted delimiter (expands):**
  ```
  echo start; cat <<EOF; echo end
  $USER $?
  EOF
  ```
  Should expand `$USER`/`$?` inside heredoc.
- **Quoted delimiter (no expand):**
  ```
  cat <<'EOF'
  $USER $?
  EOF
  ```
  Should print literal `$USER $?`.
- **SIGINT abort:**
  Start `cat <<EOF`, type a few chars, press **Ctrl‑C** → heredoc aborts, no temp leaks, `$?=130`.

#### Automated PTY tests with `expect` (optional)
Create `tests/ctrl_keys.exp`:
```tcl
#!/usr/bin/expect -f
set timeout 3
spawn ./minishell
# Ctrl-C at prompt
send 
send "echo \$?
"
expect -re {130
}
# Ctrl-C while child
send "sleep 5
"
after 300
send 
send "echo \$?
"
expect -re {130
}
# Ctrl-\ at prompt
send 
send "echo ok
"
expect -re {ok
}
# Ctrl-\ while child
send "cat
"
after 300
send 
expect -re {Quit: 3
}
send "echo \$?
"
expect -re {131
}
# Ctrl-D to exit
send 
expect eof
```
Run it:
```sh
chmod +x tests/ctrl_keys.exp
./tests/ctrl_keys.exp
```
If a check fails, `expect` will time out or mismatch the pattern.

---

## Developer Notes

* `libft/` is a **git submodule**.
* `printf_fd/` builds `libprintf_fd.a` and is linked by the top‑level Makefile.
* Object files mirror the `src/` tree; header deps generated via `-MMD -MP`.

---

## Credits

Written by Michaela and https://github.com/grignetta.
