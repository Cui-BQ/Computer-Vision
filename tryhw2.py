from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")


im = load_image("data/dog.jpg")
f = make_highpass_filter()
blu = convolve_image(im, f, 0)
clamp_image(blu)
save_image(blu, "dog-hp")