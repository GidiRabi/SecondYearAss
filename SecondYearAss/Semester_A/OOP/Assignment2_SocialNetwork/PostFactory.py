from posts.ImagePost import ImagePost
from posts.Post import Post
from posts.SalePost import SalePost
from posts.TextPost import TextPost


class PostFactory:
    @staticmethod
    def create_post(post_type: str, creator, *args) -> Post:
        if post_type == "Text":
            post = TextPost(creator, *args)
        elif post_type == "Image":
            post = ImagePost(creator, *args)
        elif post_type == "Sale":
            post = SalePost(creator, *args)
        else:
            raise ValueError(f"Invalid post type for type: {post_type}")

        print(post)
        return post
