# for type hinting
from __future__ import annotations

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from User import User

# end type hinting

import matplotlib.pyplot as plt
from PIL import Image

from posts.Post import Post


class ImagePost(Post):
    def __init__(self, creator: User, image_path: str):
        super().__init__(creator)
        self._image_url = image_path

    def display(self) -> None:
        """
        Display the image.
        Raises: FileNotFoundError if the file not found
        Returns:

        """
        print("Shows picture")
        img = Image.open(self._image_url)
        plt.imshow(img)
        plt.show()

    def __repr__(self):
        return f'{super().get_creator().username} posted a picture\n'
