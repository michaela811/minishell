# minishell (42cursus)

> **Passing the Gate.** Minishell is widely seen as the *main threshold* of 42: the first big systems project where you must design, test, and debug a real UNIX program under strict constraints (no leaks, precise signals, exact exit codes, brittle specs). Many students struggle not because it’s “hard C,” but because **small behavioral details** matter and are tested.
> Written in a group together with https://github.com/grignetta

---

## TL;DR
- **Goal:** Re‑implement a tiny Bash‑like shell.
- **You must get right:** parsing, quoting, environment/expansion, redirections, pipelines, builtins, **signals** (Ctrl‑C/D/\), **heredocs**, and **exit codes**.
- **Why this project matters:** It forces you to build a clean architecture (lexer → parser → executor) and to develop production habits: test harnesses, robust error handling, and clean resource management.

---

## Features (Mandatory Scope)
- Prompt loop using **readline** with history.
- **Lexer/Parser** producing a parse tree (or equivalent) that supports:
  - Words, operators (`|`, `<`, `>`, `<<`, `>>`), and correct **precedence**.
  - **Quotes**: single quotes = literal, double quotes = expand `$` & `$?`.
- **Expansion**: `$VAR`, `$?` (last exit status), rules with quotes.
- **Redirections**: `>`, `>>`, `<`, `<<` (heredoc with optional expansion).
- **Pipelines**: `cmd1 | cmd2 | ...` with correct FD wiring.
- **Builtins** implemented in‑process when appropriate:
  - `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Signals** & termios behavior matching Bash‐like expectations.
- **No memory leaks** over long interactive sessions; correct cleanup for all code paths.

> Bonus ideas (if allowed by your scope): logical ops `&&` / `||`, parentheses, wildcards, etc. (Not required by the classic subject.)

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

- **Executor rule of thumb:**
  - **Single builtin with redirs** → run **in parent** (so it can change shell state).
  - **Pipelines / external commands** → run in **children**.

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
- `make` – build `minishell`, `libft.a`, and `libprintf_fd.a`.
- `make clean|fclean|re` – standard hygiene.

> If you forgot submodules: `git submodule update --init --recursive`.

---

## The Controls You Must Pass (Signals & TTY behavior)
Getting **Ctrl keys** right is a common failure point. The parent shell and its children must behave differently.

### Ctrl‑C (SIGINT)
- **At prompt (no child running):**
  - Effect: print a newline, **redisplay prompt**, don’t exit.
  - **Exit status** becomes `130` for `$?`.
  - Implementation: handle SIGINT in the parent (reset current line via readline hooks), do **not** terminate the shell.
- **While a child runs:**
  - The **child** receives SIGINT with *default* disposition and terminates.
  - The **parent** prints a newline and sets last status to **`130`** (128 + SIGINT). The shell itself stays alive and shows the next prompt.

**What this means in practice**
- The terminal sends SIGINT to the **foreground process group** (your shell + its children).
- To avoid killing the shell, **parent** installs a custom handler (or temporarily ignores SIGINT) while it **waits** for the child.
- The **child** must have `SIGINT`/`SIGQUIT` restored to **default** before `execve()` so external programs behave normally.
- After `waitpid()`, if the child died from SIGINT, set `$? = 130` and print a newline to keep the prompt on a fresh line.

**Pipelines**
- All children in the pipeline receive SIGINT. Wait for each PID and set `$?` to the **last command’s** status (commonly `130` if the pipeline was interrupted).

---

### Testing

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
  ```
---

## Heredoc Rules That Are Tested
- **Delimiter quoting controls expansion**:
  - Unquoted delimiter → expand `$VAR` and `$?` inside heredoc.
  - Quoted delimiter → **no expansion**.
- **Signals inside heredoc**:
  - `Ctrl‑C` must **abort** the heredoc, clean temp files, and set status `130`.
- Use a temp file or pipe; ensure **FD hygiene** in pipelines.

---

## Exit Codes You’ll Be Graded On
- `0` – success.
- `126` – found but **not executable** (permissions / directory exec attempt).
- `127` – **command not found**.
- `130` – terminated by **SIGINT**.
- `131` – terminated by **SIGQUIT**.

Also ensure `exit` builtin accepts: `exit`, `exit <n>`, error on too many args, numeric parsing as Bash.

---

## Redirections & Pipelines – Common Edge Cases
- Redirect errors (e.g., `cat < nofile`) must not create processes needlessly; print error and set correct status.
- Mix of redirs & pipelines: last command’s exit status propagates to `$?`.
- `cd` with redirs (e.g., `cd > file`) must still run in parent when possible; handle errors before state changes.

---

## Expansion & Quotes – Quick Rules
- Single quotes `'...'` → literal.
- Double quotes `"..."` → expand `$VAR` and `$?`; keep spaces as literal characters.
- Unquoted words are subject to splitting by the parser **before** expansion is resolved into argv; design your lexer/parser so that quotes affect tokenization, not post‑processing hacks.

---

## Memory Hygiene & Leak Suppressions (Valgrind / ASan)
`readline` and other libs often keep allocations until process exit, which show up as leaks unless you filter them. The goal here is to prove **your code** is leak‑free while ignoring 3rd‑party internals.

### Valgrind (Linux) — recommended
**Typical command you can copy‑paste:**
```sh
valgrind \
  --suppressions=tools/suppress/suppress_lnx \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --trace-children=yes \
  --track-fds=yes \
  ./minishell
```
**What each flag does:**
- `--suppressions=FILE` – ignore known leaks from libraries (e.g., readline). Put your rules in `tools/suppress/suppress_lnx`.
- `--leak-check=full` – show a full backtrace for each leak.
- `--show-leak-kinds=all` – report **definite / indirect / possible / reachable** (see below).
- `--track-origins=yes` – trace uninitialized values to their origin (slower but super useful).
- `--trace-children=yes` – follow fork/exec children. Good for minishell because you spawn processes (pipes, heredoc helpers). If you only want to check the parent shell, set this to `no`.
- `--track-fds=yes` – report file descriptors not closed at exit.

**Understanding leak kinds:**
- **definitely lost** – real leaks. Fix these.
- **indirectly lost** – leaked via a leaked parent block. Usually fix.
- **possibly lost** – suspicious pointer arithmetic; investigate.
- **still reachable** – memory held until exit (often okay for libs like readline).

> **Rule of thumb:** your grade depends on fixing **definite/indirect** leaks and FD leaks in your code paths (normal run, errors, heredoc abort). Suppress (or tolerate as *reachable*) what comes from libraries you don’t control.

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

## Developer Notes
- `libft/` is a **git submodule**.
- `printf_fd/` builds `libprintf_fd.a` and is linked by the top‑level Makefile.
- Object files mirror the `src/` tree; header deps generated via `-MMD -MP`.

---

## Credits
Written by Michaela and https://github.com/grignetta.
