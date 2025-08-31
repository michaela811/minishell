# libft

My custom implementation of standard C library functions, created for 42 school projects. It builds a static library `libft.a` that can be reused easily in other projects.

---

## Features

* Re-implementation of standard libc functions (`mem*`, `str*`, `is*`, `to*`, etc.)
* Extra utilities (`ft_substr`, `ft_split`, `ft_itoa`, linked list helpers)
* Norm-compliant and leak-free

---

## Build

```bash
git clone git@github.com:grignetta/42_libft.git
cd libft
make
```
---

## Using in another project

Add `libft` as a **submodule**:

```bash
git submodule add git@github.com:grignetta/42_libft.git libft
git submodule update --init --recursive
```
Update libft to the newest version:
```bash
git submodule update --remote libft
```

Update your project Makefile:

```make
LIBFT_DIR := ./libft
LIBFT := $(LIBFT_DIR)/libft.a
INCS := -I$(LIBFT_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all
```

Then link with `$(LIBFT)`.
