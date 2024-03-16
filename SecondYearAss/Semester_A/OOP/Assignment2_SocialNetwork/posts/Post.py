# for type hinting
from __future__ import annotations

from typing import TYPE_CHECKING, Set

if TYPE_CHECKING:
    from User import User

# end type hinting
from abc import ABC, abstractmethod

from CostomExcption import NotLoginError
from Notificator import Notificator


class Post(ABC):
    _notificator = Notificator()

    @abstractmethod
    def __init__(self, creator: User) -> None:
        self._creator = creator
        self._likes: Set = set()  # set of users objects

        Post._notificator.notify_all_followers(creator, f'{creator.username} has a new post')

    def like(self, user: User) -> None:
        Post.check_if_user_login(user)

        if user not in self._likes:
            self._likes.add(user)

            Post._notificator.notify_user(self._creator, user, f'{user.username} liked your post', True)

    def comment(self, user: User, comment: str) -> None:
        Post.check_if_user_login(user)

        Post._notificator.notify_user(self._creator, user, f'{user.username} commented on your post', True,
                                      f": {comment}")

    def get_creator(self):
        return self._creator

    @staticmethod
    def check_if_user_login(user: User) -> None:
        from SocialNetwork import SocialNetwork
        # only logged-in users can do this
        if not SocialNetwork.get_instance().is_user_logged_in(user):
            raise NotLoginError("User is not logged in")

    def __str__(self):
        return self.__repr__()
